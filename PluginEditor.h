#pragma once

#include "PluginProcessor.h"
#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include "Sync.h"

class MidiXEditor : public juce::AudioProcessorEditor
{
public:
    explicit MidiXEditor(MidiX& processorToUse)
        : juce::AudioProcessorEditor(processorToUse)
        , genericEditor(processorToUse)

    {
        //addAndMakeVisible(genericEditor);
        setSize(400, 300);
    }

    void paint(juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText("version 0.0.0", getLocalBounds(), juce::Justification::topRight, 1);
    }


    void resized() override { genericEditor.setBounds(getLocalBounds()); }
    //AtomicLabel positionLabel;

    juce::GenericAudioProcessorEditor genericEditor;
};
