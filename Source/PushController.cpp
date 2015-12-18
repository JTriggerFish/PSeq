/*
  ==============================================================================

    PushController.cpp
    Created: 3 Aug 2015 3:05:41pm
    Author:  JTriggerFish

  ==============================================================================
*/

#include "PushController.h"

namespace Push
{
	MidiMessage padChangeMessage(uint8 row, uint8 col, uint8 color)
	{
		jassert(col < 8);
		jassert(row < 8);

		return MidiMessage::noteOn(1, 36 + col + row * 8, color);
	}
	MidiMessage topRowChangeMessage(uint8 col, uint8 color)
	{
		jassert(col < 8);

		return MidiMessage::controllerEvent(1, 20 + col, color);
	}
	MidiMessage bottomRowChangeMessage(uint8 col, uint8 color)
	{
		jassert(col < 8);

		return MidiMessage::controllerEvent(1, 102 + col, color);
	}
	MidiMessage sceneButtonChangeMessage(uint8 num, uint8 color)
	{
		jassert(num < 8);

		return MidiMessage::controllerEvent(1, 36 + num, color);
	}
	MidiMessage buttonChangeMessage(Identifier button, uint8 state)
	{
		return MidiMessage::controllerEvent(1, Buttons::buttonsToMidiCC()[button], state);
	}
	MidiMessage displayChangeMessage(uint8 line, String text)
	{
		static const int   maxStringSize = 68;
		static const int   messageSize   = 75; // Note the total message size is 77 with the header and tail bytes that createSysExMessage adds

		jassert(line < 4);
		jassert(text.length() < maxStringSize);

		HeapBlock<uint8> m((size_t)messageSize);

		m[0] = 0x47;
		m[1] = 0x7f;
		m[2] = 0x15;

		m[3] = 24 + line;

		m[4] = 0x0;
		m[5] = 0x45;
		m[6] = 0x0;
		
		int i = 7;
		for (auto c = 0; c < std::min(text.length(), maxStringSize ); ++c)
		{
			m[i] = static_cast<uint8>(text[i] & 127);
			++i;
		}
		for (; i < messageSize; ++i)
			m[i] = ' '; //Pad with spaces

		return MidiMessage::createSysExMessage(m, messageSize);
	}
    
    //PushControllerHandle static initializations
    ReferenceCountedObjectPtr<PushControllerHandle> PushControllerHandle::_singleInstance;
    bool                                            PushControllerHandle::isControllerConnected = false;

    //PushState static initializations
    const Identifier PushState::ControllerState     = Identifier("ControllerState");
    const Identifier PushState::PadsState           = Identifier("PadsState");
    const Identifier PushState::TopRowState         = Identifier("TopRowState");
    const Identifier PushState::BottomRowState      = Identifier("BottomRowState");
    const Identifier PushState::SceneButtonsState   = Identifier("SceneButtonsState");
    const Identifier PushState::ButtonsState        = Identifier("ButtonsState");
    const Identifier PushState::DisplayState        = Identifier("DisplayState");
    
    const Identifier PushState::Color               = Identifier("Color");
    const Identifier PushState::ButtonState         = Identifier("ButtonState");
    const Identifier PushState::IsPressed           = Identifier("IsPressed");
    const Identifier PushState::LineText            = Identifier("LineText");
    
    const Identifier PushState::Handlers            = Identifier("Handlers");

	HashMap<String, PushState::EventHandlerFunc>    PushState::eventHandlers;
    PushState                                       PushState::activeState;

    void PushControllerHandle::connectDevice()
    {
        //Singleton constructor. 
        StringArray inList  = MidiInput::getDevices();
        StringArray outList = MidiOutput::getDevices();

        int inControl  = -1;
        int outDisplay = -1;
        int outControl = -1;

        //TODO: Wild card expressions need to be improved here !
        for (int i = 0; i < inList.size(); ++i)
        {
            if (inList[i].matchesWildcard("*User Port*", true))
                inControl = i;
        }
        for (int i = 0; i < outList.size(); ++i)
        {
            if (outList[i].matchesWildcard("*User Port*", true))
                outControl = i;
            else if (outList[i].matchesWildcard("*Live Port*", true))
                outDisplay = i;
        }
        if (inControl < 0 || outControl < 0 || outDisplay < 0)
            return; //Didn't find the controller !

        controlIn  = std::shared_ptr<MidiInput>(MidiInput::openDevice(inControl, this));
        controlOut = std::shared_ptr<MidiOutput>(MidiOutput::openDevice(outControl));
        displayOut = std::shared_ptr<MidiOutput>(MidiOutput::openDevice(outDisplay));
        
        controlIn->start();
        controlOut->startBackgroundThread();
        displayOut->startBackgroundThread();

        isControllerConnected = true;
    }
    void PushControllerHandle::disconnectDevice()
    {
        if (!isControllerConnected)
            return;

        controlIn->stop();
        controlOut->stopBackgroundThread();
        displayOut->stopBackgroundThread();

        controlIn.reset();
        controlOut.reset();
        displayOut.reset();

        isControllerConnected = false;
    }

    PushControllerHandle::PushControllerHandle() : sampleRate(44100.)
    {
        connectDevice();
    }
    void PushControllerHandle::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
    {
        PushState::getActiveState().handleMidiInputEvent(source, message);
    }
    void PushControllerHandle::sendPadsStates()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        Array<var>* colors = currentState.getChildWithName(PushState::PadsState)[PushState::Color].getArray();
        //TODO: right now we have to refresh all the colors if one changes, check whether this is an issue
        for (uint8 r = 0; r < 8; ++r)
        {
            for (uint8 c = 0; c < 8; ++c)
                msgBlock.addEvent(padChangeMessage(r, c, int((*colors)[r * 8 + c])), 0);
        }
        controlOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }
    void PushControllerHandle::sendTopRowStates()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        Array<var>* colors = currentState.getChildWithName(PushState::TopRowState)[PushState::Color].getArray();
        for (uint8 r = 0; r < 8; ++r)
        {
            msgBlock.addEvent(topRowChangeMessage(r, int((*colors)[r])), 0);
        }
        controlOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }
    void PushControllerHandle::sendBottomRowStates()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        Array<var>* colors = currentState.getChildWithName(PushState::BottomRowState)[PushState::Color].getArray();
        for (uint8 r = 0; r < 8; ++r)
        {
            msgBlock.addEvent(bottomRowChangeMessage(r, int((*colors)[r])), 0);
        }
        controlOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }
    void PushControllerHandle::sendSceneButtonsStates()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        Array<var>* colors = currentState.getChildWithName(PushState::SceneButtonsState)[PushState::Color].getArray();
        for (uint8 r = 0; r < 8; ++r)
        {
            msgBlock.addEvent(sceneButtonChangeMessage(r, int((*colors)[r])), 0);
        }
        controlOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }
    void PushControllerHandle::sendButtonsStates()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        auto buttonStates = currentState.getChildWithName(PushState::ButtonsState);
        for (auto b : Push::Buttons::allButtons)
        {
            ValueTree buttonValueTree = buttonStates.getChildWithName(b);
            msgBlock.addEvent(buttonChangeMessage(b, int(buttonValueTree.getProperty(PushState::ButtonState))), 0);
        }
        controlOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }
    void PushControllerHandle::sendDisplayState()
    {
        ValueTree& currentState = PushState::getActiveState();
        MidiBuffer msgBlock;
        double millisecondCounterToStartAt = Time::getMillisecondCounter();

        ValueTree displayState = currentState.getChildWithName(PushState::DisplayState);
        auto lines = displayState[PushState::LineText];
        for (uint8 l = 0; l < 4; ++l)
            msgBlock.addEvent(displayChangeMessage(l, lines[l]), 0);

        displayOut->sendBlockOfMessages(msgBlock, millisecondCounterToStartAt, sampleRate);
    }

    void PushControllerHandle::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
    {
        ValueTree& currentState = PushState::getActiveState();
        if (treeWhosePropertyHasChanged == currentState.getChildWithName(PushState::PadsState))
        {
            if (property == PushState::Color)
                sendPadsStates();
        }
        else if (treeWhosePropertyHasChanged == currentState.getChildWithName(PushState::TopRowState))
        {
            if (property == PushState::Color)
                sendTopRowStates();
        }
        else if (treeWhosePropertyHasChanged == currentState.getChildWithName(PushState::BottomRowState))
        {
            if (property == PushState::Color)
                sendBottomRowStates();
        }
        else if (treeWhosePropertyHasChanged == currentState.getChildWithName(PushState::SceneButtonsState))
        {
            if (property == PushState::Color)
                sendSceneButtonsStates();
        }
        else if (treeWhosePropertyHasChanged == currentState.getChildWithName(PushState::DisplayState))
        {
            sendDisplayState();
        }
        else
        {
            //Should be one of the buttons then, since each has its own value tree
            if (property == PushState::ButtonState)
                sendButtonsStates();
        }
    }
    void PushControllerHandle::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
    {
        //Nothing for now
    }
    void PushControllerHandle::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int oldIndex)
    {
        //Nothing for now
    }
    void PushControllerHandle::valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
    {
        //Nothing for now
    }
    void PushControllerHandle::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged)
    {
        //Nothing for now
    }
    void PushControllerHandle::valueTreeRedirected(ValueTree& treeWhichHasBeenChanged)
    {
        //For now just refresh all
        sendPadsStates();
        sendTopRowStates();
        sendBottomRowStates();
        sendSceneButtonsStates();
        sendButtonsStates();
        sendDisplayState();
    }
    
    ValueTree PushState::createNewDefaultState()
    {
        ValueTree state(ControllerState);
        {
            ValueTree padsState(PadsState);
            padsState.setProperty(Color, Array<var>(), nullptr);
            auto colors = padsState[Color];
            colors.resize(8*8);
            for(auto i =0; i < colors.size(); ++i)
                colors[i] = Push::PadColors::black;
            state.addChild(padsState, -1, nullptr);
        }
        {
            ValueTree topRowState(TopRowState);
            topRowState.setProperty(Color, Array<var>(), nullptr);
            auto colors = topRowState[Color];
            colors.resize(8);
            for(auto i = 0; i < colors.size(); ++i)
                colors[i] = Push::TopRowColors::black;
			topRowState.setProperty(IsPressed, Array<var>(), nullptr);
			auto isPressed = topRowState[IsPressed];
			isPressed.resize(8);
			for (auto i = 0; i < isPressed.size(); ++i)
				isPressed[i] = false;
            state.addChild(topRowState, -1, nullptr);
		}
		{
			ValueTree bottomRowState(BottomRowState);
			bottomRowState.setProperty(Color, Array<var>(), nullptr);
			auto colors = bottomRowState[Color];
			colors.resize(8);
			for (auto i = 0; i < colors.size(); ++i)
				colors[i] = Push::TopRowColors::black;
			bottomRowState.setProperty(IsPressed, Array<var>(), nullptr);
			auto isPressed = bottomRowState[IsPressed];
			isPressed.resize(8);
			for (auto i = 0; i < isPressed.size(); ++i)
				isPressed[i] = false;
			state.addChild(bottomRowState, -1, nullptr);
		}
		{
			ValueTree sceneButtonsState(SceneButtonsState);
			sceneButtonsState.setProperty(Color, Array<var>(), nullptr);
			auto colors = sceneButtonsState[Color];
			colors.resize(8);
			for (auto i = 0; i < colors.size(); ++i)
				colors[i] = Push::SceneColors::red;
			sceneButtonsState.setProperty(IsPressed, Array<var>(), nullptr);
			auto isPressed = sceneButtonsState[IsPressed];
			isPressed.resize(8);
			for (auto i = 0; i < isPressed.size(); ++i)
				isPressed[i] = false;
			state.addChild(sceneButtonsState, -1, nullptr);
		}
		{
			ValueTree buttonsState(ButtonsState);
			for (auto i : Push::Buttons::allButtons)
			{
				ValueTree bt(i);
				bt.setProperty(ButtonState, Push::Buttons::On, nullptr);
				bt.setProperty(IsPressed, false, nullptr);
				buttonsState.addChild(bt, -1, nullptr);
			}
			state.addChild(buttonsState, -1, nullptr);
		}
		{
			ValueTree displayState(DisplayState);
			displayState.setProperty(LineText, Array<var>(), nullptr);
			auto lines = displayState[LineText];
			lines.resize(4);
			for (auto i = 0; i < lines.size(); ++i)
				lines[i] = "";
			state.addChild(displayState, -1, nullptr);
		}
        
        //Do nothing with incoming events from the hardware by default.
        state.setProperty(Handlers, Array<var>(), nullptr);

        
        return state;
    }
    void PushState::setActiveState(PushState& _state)
    {
        //Note: this should trigger the listener to send the state changes to the hardware
        //TODO: check this works properly !
        //otherwise will need to manually flush the state
        activeState.state = _state.state;
    }
    void PushState::turnOnListener()
    {
        activeState.state.addListener(PushControllerHandle::getHandle());
    }
	void PushState::addHandler(String handlerName, int pos)
	{
		auto handlers = state[Handlers];
		if (pos > 0)
			handlers.insert(pos, handlerName);
		else
			handlers.append(handlerName);
	}
    void PushState::handleMidiInputEvent(MidiInput *source, const MidiMessage &message)
    {
        HashMap<String, EventHandlerFunc>::Iterator i(eventHandlers);
        while (i.next())
        {
            auto _name = i.getKey();
            auto func  = i.getValue();
            func(message, state);
        }
    }
	void PushState::registerHandler(String name, const EventHandlerFunc&& func)
	{
		if (eventHandlers.contains(name))
			return;
		eventHandlers.set(name, func);
	}
	bool PushState::checkIfHandlerExists(String name)
	{
		return (eventHandlers.contains(name));
	}

}