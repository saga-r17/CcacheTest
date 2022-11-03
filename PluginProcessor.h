
#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include "Sync.h"


class MidiX : public PluginHelpers::ProcessorBase
{
public:

    MidiX();
    
    AudioProcessor(BusesProperties());
          // add no audio buses at all
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;

    std::atomic<double> currentPosition{0};
    
    Sync sync;
    WhiteNoise::Oscillator noise;
    double rate;
    bool flag = false;
    juce::Label versionLabel;

private:

    juce::AudioParameterFloat* NoteDuration;
    int currentNote, lastNoteValue, tempSample;
    int time;
    juce::SortedSet<int> notes;

};



