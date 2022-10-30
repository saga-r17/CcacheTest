/*
  ==============================================================================

    LooperData.h
    Created: 11 Oct 2022 10:16:16am
    Author:  Gustavo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class LooperData {
public:
    LooperData();
    void update();

    //juce::MidiMessageSequence midiSequence;
    std::unique_ptr<juce::MidiMessageSequence> midiSequence;


    void NoteBoxChanged(juce::String text, juce::String keyText);

    void LoopEndBoxChanged(juce::String text, juce::AudioProcessorValueTreeState& apvts);


    void NotationBoxChanged(const int notation);

    int KeyNameToNumber(const juce::String& keyName);

private:
};