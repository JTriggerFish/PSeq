/*
  ==============================================================================

    PushControllerButtons.cpp
    Created: 4 Aug 2015 10:24:50am
    Author:  JTriggerFish

  ==============================================================================
*/

#include "PushControllerButtons.h"

namespace Push
{
	DefaultHashFunctions Push::IdentifierHash::func = DefaultHashFunctions();

	const Identifier Buttons::Tap		  = Identifier("Tap");
	const Identifier Buttons::Metronome   = Identifier("Metronome");
	const Identifier Buttons::Master	  = Identifier("Master");
	const Identifier Buttons::Stop		  = Identifier("Stop");
	const Identifier Buttons::Left		  = Identifier("Left");
	const Identifier Buttons::Right		  = Identifier("Right");
	const Identifier Buttons::Up		  = Identifier("Up");
	const Identifier Buttons::Down		  = Identifier("Down");
	const Identifier Buttons::Select	  = Identifier("Select");
	const Identifier Buttons::Shift		  = Identifier("Shift");
	const Identifier Buttons::Note		  = Identifier("Note");
	const Identifier Buttons::Session	  = Identifier("Session");
	const Identifier Buttons::AddEffect   = Identifier("AddEffect");
	const Identifier Buttons::AddTrack	  = Identifier("AddTrack");
	const Identifier Buttons::OctaveDown  = Identifier("OctaveDown");
	const Identifier Buttons::OctaveUp	  = Identifier("OctaveUp");
	const Identifier Buttons::Repeat	  = Identifier("Repeat");
	const Identifier Buttons::Accent	  = Identifier("Accent");
	const Identifier Buttons::Scales	  = Identifier("Scales");
	const Identifier Buttons::UserMode	  = Identifier("UserMode");
	const Identifier Buttons::Mute		  = Identifier("Mute");
	const Identifier Buttons::Solo	      = Identifier("Solo");
	const Identifier Buttons::DeviceLeft  = Identifier("DeviceLeft");
	const Identifier Buttons::DeviceRight = Identifier("DeviceRight");
	const Identifier Buttons::Play		  = Identifier("Play");
	const Identifier Buttons::Record	  = Identifier("Record");
	const Identifier Buttons::New		  = Identifier("New");
	const Identifier Buttons::Duplicate   = Identifier("Duplicate");
	const Identifier Buttons::Automation  = Identifier("Automation");
	const Identifier Buttons::FixedLength = Identifier("FixedLength");
	const Identifier Buttons::Device	  = Identifier("Device");
	const Identifier Buttons::Browse	  = Identifier("Browse");
	const Identifier Buttons::Track		  = Identifier("Track");
	const Identifier Buttons::Clip		  = Identifier("Clip");
	const Identifier Buttons::Volume	  = Identifier("Volume");
	const Identifier Buttons::PanSend	  = Identifier("PanSend");
	const Identifier Buttons::Quantize	  = Identifier("Quantize");
	const Identifier Buttons::Double	  = Identifier("Double");
	const Identifier Buttons::Delete	  = Identifier("Delete");
	const Identifier Buttons::Undo		  = Identifier("Undo");

	const Identifier Buttons::allButtons[] = { "Tap", "Metronome", "Master", "Stop", "Left", "Right", "Up", "Down",
		"Select", "Shift", "Note", "Session", "AddEffect", "AddTrack", "OctaveDown",
		"OctaveUp", "Repeat", "Accent", "Scales", "UserMode", "Mute", "Solo", "DeviceLeft",
		"DeviceRight", "Play", "Record", "New", "Duplicate", "Automation", "FixedLength", "Device",
		"Browse", "Track", "Clip", "Volume", "PanSend", "Quantize", "Double", "Delete", "Undo" };


	HashMap<Identifier, uint8, IdentifierHash> Push::Buttons::_buttonsToMidiCC;
	bool Buttons::_isMapInitialized = false;

	const HashMap<Identifier, uint8, IdentifierHash>& Buttons::buttonsToMidiCC()
	{
		if (_isMapInitialized)
		{
			_buttonsToMidiCC.set(Tap, 3);
			_buttonsToMidiCC.set(Metronome, 9);
			_buttonsToMidiCC.set(Master, 28);
			_buttonsToMidiCC.set(Stop, 29);
			_buttonsToMidiCC.set(Left, 44);
			_buttonsToMidiCC.set(Right, 45);
			_buttonsToMidiCC.set(Up, 46);
			_buttonsToMidiCC.set(Down, 47);
			_buttonsToMidiCC.set(Select, 48);
			_buttonsToMidiCC.set(Shift, 49);
			_buttonsToMidiCC.set(Note, 50);
			_buttonsToMidiCC.set(Session, 51);
			_buttonsToMidiCC.set(AddEffect, 52);
			_buttonsToMidiCC.set(AddTrack, 53);
			_buttonsToMidiCC.set(OctaveDown, 54);
			_buttonsToMidiCC.set(OctaveUp, 55);
			_buttonsToMidiCC.set(Repeat, 56);
			_buttonsToMidiCC.set(Accent, 57);
			_buttonsToMidiCC.set(Scales, 58);
			_buttonsToMidiCC.set(UserMode, 59);
			_buttonsToMidiCC.set(Mute, 60);
			_buttonsToMidiCC.set(Solo, 61);
			_buttonsToMidiCC.set(DeviceLeft, 62);
			_buttonsToMidiCC.set(DeviceRight, 63);
			_buttonsToMidiCC.set(Play, 85);
			_buttonsToMidiCC.set(Record, 86);
			_buttonsToMidiCC.set(New, 87);
			_buttonsToMidiCC.set(Duplicate, 88);
			_buttonsToMidiCC.set(Automation, 89);
			_buttonsToMidiCC.set(FixedLength, 90);
			_buttonsToMidiCC.set(Device, 110);
			_buttonsToMidiCC.set(Browse, 111);
			_buttonsToMidiCC.set(Track, 112);
			_buttonsToMidiCC.set(Clip, 113);
			_buttonsToMidiCC.set(Volume, 114);
			_buttonsToMidiCC.set(PanSend, 115);
			_buttonsToMidiCC.set(Quantize, 116);
			_buttonsToMidiCC.set(Double, 117);
			_buttonsToMidiCC.set(Delete, 118);
			_buttonsToMidiCC.set(Undo, 11);

			_isMapInitialized = true;
		}
		return _buttonsToMidiCC;
	}

}