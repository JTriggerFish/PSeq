/*
  ==============================================================================

  PushController.h
  Created: 3 Aug 2015 3:05:41pm
  Author:  JTriggerFish

  ==============================================================================
  */

#ifndef PUSHCONTROLLER_H_INCLUDED
#define PUSHCONTROLLER_H_INCLUDED

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
								 public virtual ValueTree::Listener
	{
	public:
		static ReferenceCountedObjectPtr<PushControllerHandle> getHandle();

	private:
		PushControllerHandle() {}
		PushControllerHandle(const PushControllerHandle&) {}
		PushControllerHandle& operator=(const PushControllerHandle&) {}

		static ReferenceCountedObjectPtr<PushControllerHandle> _singleInstance;
	};
    
    /** Class encapsulating the ValueTree that represents the controller state and related static methdods and constants */
    class PushState
    {
    public:
        typedef std::function<void(MidiMessage,juce::ValueTree)> EventHandlerFunc;
        
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
        
        //Current active state
        static juce::ValueTree activeState;
        
		static void setActiveState(juce::ValueTree state) { activeState = state; }
		static void setActiveStateListener(ReferenceCountedObjectPtr<PushControllerHandle>&&  listener) { activeStateListener = listener; }
        
        /** The class is a thin wrapper around a ValueTree intended to add some convenience functions.
         *  The constructor creates a default empty state with neutral buttons and screen
         *  and no handlers */
        PushState()
        {
            state = createNewDefaultState();
        }
        virtual ~PushState() {}
        operator juce::ValueTree() { return state; }
        /** Add an event handler to a state. Note that handlers are executed in order and thus
        the position matters since handlers may modify the state. By default insert at the end of the 
         list */
		void addHandler(String handlerName, int pos = -1);
        
        static juce::ValueTree createNewDefaultState();
        /** Add a handler to the repository. Nothing will be done if there is an existing
         *  handler with this identifier */
        static void registerHandler(String name, const EventHandlerFunc&& func);
        static bool checkIfHandlerExists(String name);
        
        
    private:
        //Event handler repository
        static HashMap<String,EventHandlerFunc>				   eventHandlers;
        static ReferenceCountedObjectPtr<PushControllerHandle> activeStateListener;
        
        juce::ValueTree state;
    };
}



#endif  // PUSHCONTROLLER_H_INCLUDED
