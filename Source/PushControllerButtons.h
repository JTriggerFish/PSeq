/*
  ==============================================================================

    PushControllerButtons.h
    Created: 4 Aug 2015 10:24:50am
    Author:  JTriggerFish

  ==============================================================================
*/

#ifndef PUSHCONTROLLERBUTTONS_H_INCLUDED
#define PUSHCONTROLLERBUTTONS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace Push
{
	class Buttons
	{
	public:
		static const Identifier Tap;
		static const Identifier Metronome;
		static const Identifier Master;
		static const Identifier Stop;
		static const Identifier Left;
		static const Identifier Right;
		static const Identifier Up;
		static const Identifier Down;
		static const Identifier Select;
		static const Identifier Shift;
		static const Identifier Note;
		static const Identifier Session;
		static const Identifier AddEffect;
		static const Identifier AddTrack;
		static const Identifier OctaveDown;
		static const Identifier OctaveUp;
		static const Identifier Repeat;
		static const Identifier Accent;
		static const Identifier Scales;
		static const Identifier UserMode;
		static const Identifier Mute;
		static const Identifier Solo;
		static const Identifier DeviceLeft;
		static const Identifier DeviceRight;
		static const Identifier Play;
		static const Identifier Record;
		static const Identifier New;
		static const Identifier Duplicate;
		static const Identifier Automation;
		static const Identifier FixedLength;
		static const Identifier Device;
		static const Identifier Browse;
		static const Identifier Track;
		static const Identifier Clip;
		static const Identifier Volume;
		static const Identifier PanSend;
		static const Identifier Quantize;
		static const Identifier Double;
		static const Identifier Delete;
		static const Identifier Undo;

		enum States  { Off = 0, On = 1, Hi = 4 };


		struct HashGenerator
		{
			static DefaultHashFunctions func;
			int generateHash(Identifier key, int upperLimit) const
			{
				return func.generateHash(key.toString(), upperLimit);
			}
		};
		static const Identifier			         allButtons[];
		static const HashMap<Identifier, uint8, HashGenerator>& buttonsToMidiCC();

	private:
		static HashMap<Identifier, uint8, HashGenerator> _buttonsToMidiCC;
		static bool					      _isMapInitialized;
	};
}



#endif  // PUSHCONTROLLERBUTTONS_H_INCLUDED
