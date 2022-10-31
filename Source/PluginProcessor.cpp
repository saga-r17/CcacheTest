/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MASCAudioProcessor::MASCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

MASCAudioProcessor::~MASCAudioProcessor()
{
}

//==============================================================================
const juce::String MASCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MASCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MASCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MASCAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MASCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MASCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MASCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MASCAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MASCAudioProcessor::getProgramName (int index)
{
    return {};
}

void MASCAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MASCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MASCAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MASCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MASCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    midiMessages.clear();

    midiProcessor.process(buffer,midiMessages,apvts);
    
    if(midiMessages.getNumEvents()>0){
        juce::MidiBuffer::Iterator it(midiMessages);
        juce::MidiMessage currentMessage;
        int samplePos;
        while (it.getNextEvent(currentMessage, samplePos)) {
            if(currentMessage.isNoteOnOrOff())
            juce::Logger::writeToLog(currentMessage.getDescription());
        }
    }

}

//==============================================================================
bool MASCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MASCAudioProcessor::createEditor()
{
    return new MASCAudioProcessorEditor (*this);
}

//==============================================================================
void MASCAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MASCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout MASCAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //Notation Selection
    params.push_back(std::make_unique<juce::AudioParameterChoice>("NOTBOX", "Notation Box", juce::StringArray{ "4n","8n","16n" }, 0));
    
    //TODO: DEFINE MAX VALUE
    params.push_back(std::make_unique<juce::AudioParameterInt>("ENDBAR", "Loop End Bar", 0, 100, 4));
    params.push_back(std::make_unique<juce::AudioParameterInt>("END16", "Loop End 16n", 0, 100, 0));

    return {params.begin(),params.end()};
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MASCAudioProcessor();
}
