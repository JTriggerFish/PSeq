/*
  ==============================================================================

  PushController.h
  Created: 3 Aug 2015 3:05:41pm
  Author:  JTriggerFish

  ==============================================================================
  */

#ifndef PUSHCONTROLLER_H_INCLUDED
#define PUSHCONTROLLER_H_INCLUDED

#include <memory>
#include "PushControllerColors.h"
#include "PushControllerButtons.h"

namespace Push
{
	/** Methods to convert state changes to send to the push controller to midi messages
	 For now it's color and display strings but in the future we will be adding things like velocity curves
	*/
	MidiMessage padChangeMessage (uint8 row, uint8 col, uint8 color);
	MidiMessage topRowChangeMessage (uint8 col, uint8 color);
	MidiMessage bottomRowChangeMessage (uint8 col, uint8 color);
	MidiMessage sceneButtonChangeMessage (uint8 num, uint8 color);
	MidiMessage buttonChangeMessage (Identifier button, uint8 state);
	MidiMessage displayChangeMessage (uint8 line, String text);

	/** Handle to controller hardware, singleton class
	*/
	class PushControllerHandle : public ReferenceCountedObject,
								 public virtual ValueTree::Listener,
                                 public MidiInputCallback
	{
	public:
        static ReferenceCountedObjectPtr<PushControllerHandle> getHandle()
        {
            if (!_singleInstance)
                _singleInstance = ReferenceCountedObjectPtr<PushControllerHandle>(new PushControllerHandle());
            return _singleInstance;
        }
        //Implement ValueTree::Listener
        void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int oldIndex) override;
        void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
        void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override;
        
        //Implement MidiInputCallback
        void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) override;
        void handlePartialSysexMessage (MidiInput *source, const uint8 *messageData, int numBytesSoFar, double timestamp) override {}

        void connectDevice();
        void disconnectDevice();

        //TODO SET ME !
        void setSampleRate(double sr) { sampleRate = sr; }

        ~PushControllerHandle() { disconnectDevice(); }

	private:
        PushControllerHandle();
		PushControllerHandle(const PushControllerHandle&) {}
        PushControllerHandle& operator=(const PushControllerHandle&) {return *this;}
        
        void sendPadsStates();
        void sendTopRowStates();
        void sendBottomRowStates();
        void sendSceneButtonsStates();
        void sendButtonsStates();
        void sendDisplayState();


		static ReferenceCountedObjectPtr<PushControllerHandle> _singleInstance;
        static bool isControllerConnected;

        //Needs to be set by host / plugin !
        double    sampleRate;
        
        std::shared_ptr<MidiInput>  controlIn;
        std::shared_ptr<MidiOutput> controlOut;
        std::shared_ptr<MidiOutput> displayOut;

	};
    
    /** Class encapsulating the ValueTree that represents the controller state and related static methdods and constants */
    class PushState
    {
    public:
        typedef std::function<void(MidiMessage,juce::ValueTree&)> EventHandlerFunc;
        
        //Identifiers for the state ValueTree
        static const Identifier ControllerState;
        static const Identifier PadsState;
        static const Identifier TopRowState;
        static const Identifier BottomRowState;
        static const Identifier SceneButtonsState;
        static const Identifier ButtonsState;
        static const Identifier DisplayState;
        
        static const Identifier Color;
        static const Identifier ButtonState;
        static const Identifier IsPressed;
        static const Identifier LineText;
        
        static const Identifier Handlers;
        
        
        static void setActiveState(PushState& state);
        static PushState& getActiveState() { return activeState; }

        //Turn on the state listener by calling PushControllerHandle::getHandle()
        //and setting it as the listener to the active state
        static void turnOnListener();
        
        /** The class is a thin wrapper around a ValueTree intended to add some convenience functions.
         *  The constructor creates a default empty state with neutral buttons and screen
         *  and no handlers */
        PushState()
        {
            state = createNewDefaultState();
        }
        virtual ~PushState() {}
        operator juce::ValueTree&() { return state; }
        /** Add an event handler to a state. Note that handlers are executed in order and thus
        the position matters since handlers may modify the state. By default insert at the end of the 
         list */
		void addHandler(String handlerName, int pos = -1);
        void handleMidiInputEvent(MidiInput *source, const MidiMessage &message);
        
        
        static juce::ValueTree createNewDefaultState();
        /** Add a handler to the repository. Nothing will be done if there is an existing
         *  handler with this identifier */
        static void registerHandler(String name, const EventHandlerFunc&& func);
        static bool checkIfHandlerExists(String name);

    private:
        //Event handler repository
        static HashMap<String,EventHandlerFunc>	   eventHandlers;
        juce::ValueTree state;

        //Current active state
        static PushState activeState;
    };
}



#endif  // PUSHCONTROLLER_H_INCLUDED
