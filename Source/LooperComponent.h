/*
  ==============================================================================

    LooperComponent.h
    Created: 11 Oct 2022 10:16:39am
    Author:  Gustavo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LooperData.h"

class LooperData;

class LooperComponent : public juce::Component {
public:
    LooperComponent(LooperData &looperData,juce::AudioProcessorValueTreeState& apvts);
    ~LooperComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LooperData* looperdata;
    
    void NoteBoxChanged();

    juce::Label NoteBox;

    juce::Label LoopEndBox;

    juce::Label KeyBox;

    //LoopEndBox
    void LoopEndBoxChanged(juce::AudioProcessorValueTreeState& apvts);

    //Notation Dropdown
    juce::ComboBox NotationBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> NotationAttachment;
    void NotationBoxChanged(juce::AudioProcessorValueTreeState& apvts);

};

class LoopEndFilter : public::juce::TextEditor::InputFilter {
    juce::String filterNewText(juce::TextEditor&, const juce::String& newInput) override;
};