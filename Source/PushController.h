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
    
    /** Class encapsulating the ValueTree that represents the controller state and related static methdods and constants */
    class PushState
    {
    public:
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
        
        static ValueTree createNewDefaultState();
    };
}



#endif  // PUSHCONTROLLER_H_INCLUDED
