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
	const auto Buttons::Tap		  = Identifier("Tap");
	const auto Buttons::Metronome   = Identifier("Metronome");
	const auto Buttons::Master	  = Identifier("Master");
	const auto Buttons::Stop		  = Identifier("Stop");
	const auto Buttons::Left		  = Identifier("Left");
	const auto Buttons::Right		  = Identifier("Right");
	const auto Buttons::Up		  = Identifier("Up");
	const auto Buttons::Down		  = Identifier("Down");
	const auto Buttons::Select	  = Identifier("Select");
	const auto Buttons::Shift		  = Identifier("Shift");
	const auto Buttons::Note		  = Identifier("Note");
	const auto Buttons::Session	  = Identifier("Session");
	const auto Buttons::AddEffect   = Identifier("AddEffect");
	const auto Buttons::AddTrack	  = Identifier("AddTrack");
	const auto Buttons::OctaveDown  = Identifier("OctaveDown");
	const auto Buttons::OctaveUp	  = Identifier("OctaveUp");
	const auto Buttons::Repeat	  = Identifier("Repeat");
	const auto Buttons::Accent	  = Identifier("Accent");
	const auto Buttons::Scales	  = Identifier("Scales");
	const auto Buttons::UserMode	  = Identifier("UserMode");
	const auto Buttons::Mute		  = Identifier("Mute");
	const auto Buttons::Solo	      = Identifier("Solo");
	const auto Buttons::DeviceLeft  = Identifier("DeviceLeft");
	const auto Buttons::DeviceRight = Identifier("DeviceRight");
	const auto Buttons::Play		  = Identifier("Play");
	const auto Buttons::Record	  = Identifier("Record");
	const auto Buttons::New		  = Identifier("New");
	const auto Buttons::Duplicate   = Identifier("Duplicate");
	const auto Buttons::Automation  = Identifier("Automation");
	const auto Buttons::FixedLength = Identifier("FixedLength");
	const auto Buttons::Device	  = Identifier("Device");
	const auto Buttons::Browse	  = Identifier("Browse");
	const auto Buttons::Track		  = Identifier("Track");
	const auto Buttons::Clip		  = Identifier("Clip");
	const auto Buttons::Volume	  = Identifier("Volume");
	const auto Buttons::PanSend	  = Identifier("PanSend");
	const auto Buttons::Quantize	  = Identifier("Quantize");
	const auto Buttons::Double	  = Identifier("Double");
	const auto Buttons::Delete	  = Identifier("Delete");
	const auto Buttons::Undo		  = Identifier("Undo");

	const Identifier Buttons::allButtons[] = { "Tap", "Metronome", "Master", "Stop", "Left", "Right", "Up", "Down",
		"Select", "Shift", "Note", "Session", "AddEffect", "AddTrack", "OctaveDown",
		"OctaveUp", "Repeat", "Accent", "Scales", "UserMode", "Mute", "Solo", "DeviceLeft",
		"DeviceRight", "Play", "Record", "New", "Duplicate", "Automation", "FixedLength", "Device",
		"Browse", "Track", "Clip", "Volume", "PanSend", "Quantize", "Double", "Delete", "Undo" };


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