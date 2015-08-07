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
		static const uint8 maxStringSize = 68;
		static const uint8 messageSize   = 75; // Note the total message size is 77 with the header and tail bytes that createSysExMessage adds

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
		
		size_t i = 7;
		for (auto c = 0; c < std::min((size_t)text.length(), (size_t)maxStringSize ); ++c)
		{
			m[i] = (text[i] & 127);
			++i;
		}
		for (; i < messageSize; ++i)
			m[i] = ' '; //Pad with spaces

		return MidiMessage::createSysExMessage(m, messageSize);
	}
    const auto PushState::ControllerState     = Identifier("ControllerState");
    const auto PushState::PadsState           = Identifier("PadsState");
    const auto PushState::TopRowState         = Identifier("TopRowState");
    const auto PushState::BottomRowState      = Identifier("BottomRowState");
    const auto PushState::SceneButtonsState   = Identifier("SceneButtonsState");
    const auto PushState::ButtonsState        = Identifier("ButtonsState");
    const auto PushState::DisplayState        = Identifier("DisplayState");
    
    const auto PushState::Color               = Identifier("Color");
    const auto PushState::ButtonState         = Identifier("ButtonState");
    const auto PushState::IsPressed           = Identifier("IsPressed");
    const auto PushState::LineText            = Identifier("LineText");
    
    const auto PushState::Handlers            = Identifier("Handlers");
    
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
            for(auto i =0; i < colors.size(); ++i)
                colors[i] = Push::TopRowColors::black;
            state.addChild(topRowState, -1, nullptr);
        }
        //TODO
        
        //Do nothing with events by default.
        state.setProperty(Handlers, Array<var>(), nullptr);
        
        return state;
    }

}