/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LooperComponent.h"
//==============================================================================
/**
*/
class MASCAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MASCAudioProcessorEditor (MASCAudioProcessor&);
    ~MASCAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MASCAudioProcessor& audioProcessor;
    LooperComponent looper;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MASCAudioProcessorEditor)
};
