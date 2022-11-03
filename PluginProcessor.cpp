
#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiX::MidiX()
    :PluginHelpers::ProcessorBase(getDefaultProperties())
{
    addParameter(NoteDuration = new juce::AudioParameterFloat("note length", "Note Duration", 0.0, 1.0, 0.7));
    currentNote = 0;
    lastNoteValue = -1;
    tempSample = 1;
    rate = AudioProcessor::getSampleRate();
    time = 0;
    
}

void MidiX::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sync.prepare(sampleRate, samplesPerBlock);
    juce::ignoreUnused(samplesPerBlock);
    notes.clear();
    rate = static_cast<float> (sampleRate);

}

void MidiX::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midi)
{

    DBG("message");
    // the audio buffer in a midi effect will have zero channels!
    jassert(buffer.getNumChannels() == 0);
    // using number of samples in buffer as a reference
    auto numSamples = buffer.getNumSamples();
    //juce::ignoreUnused(buffer, midiMessages);
    sync.process(getPlayHead(), buffer.getNumSamples());

    // get note duration
    auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (*NoteDuration)))));


    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())  notes.add(msg.getNoteNumber());
        else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
    }
    midi.clear();

    
    for (int sample = 0; sample < numSamples; sample++)
        {

        auto relativePosition = fmod(sync.ppqPositions[sample], 1.f);
        
        if (sync.info.isPlaying && relativePosition < 0.0001)
        {
            flag = true;
            tempSample = sample;
        }

        }
 

    if (flag && notes.size() > 0)
    {
        currentNote = (currentNote + 1) % notes.size();
        lastNoteValue = notes[currentNote];
        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), tempSample);
        flag = false;
        time = 0;
    }

    if ((time + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));

        if (lastNoteValue > 0)
        {
            midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
            lastNoteValue = -1;         
        }
   
    }

    time = (time + numSamples) % noteDuration;  
}



juce::AudioProcessorEditor* MidiX::createEditor()
{
    return new MidiXEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiX();
}
