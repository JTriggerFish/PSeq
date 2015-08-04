/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PseqAudioProcessor::PseqAudioProcessor()
{
}

PseqAudioProcessor::~PseqAudioProcessor()
{
}

//==============================================================================
const String PseqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int PseqAudioProcessor::getNumParameters()
{
    return 0;
}

float PseqAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void PseqAudioProcessor::setParameter (int index, float newValue)
{
}

const String PseqAudioProcessor::getParameterName (int index)
{
    return String();
}

const String PseqAudioProcessor::getParameterText (int index)
{
    return String();
}

const String PseqAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PseqAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PseqAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool PseqAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool PseqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PseqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PseqAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PseqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PseqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PseqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PseqAudioProcessor::setCurrentProgram (int index)
{
}

const String PseqAudioProcessor::getProgramName (int index)
{
    return String();
}

void PseqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PseqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PseqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PseqAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool PseqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PseqAudioProcessor::createEditor()
{
    return new PseqAudioProcessorEditor (*this);
}

//==============================================================================
void PseqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PseqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PseqAudioProcessor();
}