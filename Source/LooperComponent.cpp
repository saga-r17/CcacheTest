/*
  ==============================================================================

    LooperComponent.cpp
    Created: 11 Oct 2022 10:16:39am
    Author:  Gustavo

  ==============================================================================
*/

#include "LooperComponent.h"
#include "LooperData.h"
#include <regex>

LooperComponent::LooperComponent(LooperData& looperData,juce::AudioProcessorValueTreeState& apvts)
{
    this->looperdata = &looperData;

    NoteBox.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    NoteBox.setFont(juce::Font(25.0f));
    NoteBox.setText("1.1.",juce::dontSendNotification);
    NoteBox.setEditable(true);
    NoteBox.onTextChange = [&] {NoteBoxChanged(); };
    NoteBox.onEditorShow = [&] {
       juce::TextEditor* currentEditor = NoteBox.getCurrentTextEditor();
        //SETS LIMIT ANT TYPE OF CHARACTER, TODO:define macros
        currentEditor->setInputRestrictions(200, "1.");

    };

    KeyBox.setColour(juce::TextEditor::textColourId, juce::Colours::aqua);
    KeyBox.setFont(juce::Font(25.0f));
    KeyBox.setText("C4", juce::dontSendNotification);
    KeyBox.setEditable(true);
    //TODO: Optimize by creating a separate function when MIDI Key change
    KeyBox.onTextChange = [&] {NoteBoxChanged(); };
    KeyBox.onEditorShow = [&] {
        juce::TextEditor* currentEditor = KeyBox.getCurrentTextEditor();
        //SETS LIMIT ANT TYPE OF CHARACTER, TODO:define macros, make more robust (regex)
        currentEditor->setInputRestrictions(4, "ABCDEFG#b01234567890-");

    };

    //Loop End Box
    LoopEndBox.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    LoopEndBox.setFont(juce::Font(25.0f));
    LoopEndBox.setText("4:0:0", juce::dontSendNotification);
    LoopEndBox.setEditable(true);
    LoopEndBox.onTextChange = [&] {LoopEndBoxChanged(apvts); };
    LoopEndBox.onEditorShow = [&] {
        juce::TextEditor* currentEditor = LoopEndBox.getCurrentTextEditor();
        //TODO: SET AN INPUT FILTER

    };
    //Notation Box
    juce::StringArray choices{ "4n","8n","16n" };
    NotationBox.addItemList(choices, 1);
    NotationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "NOTBOX", NotationBox);

    NotationBox.onChange = [&] {NotationBoxChanged(apvts); };

    addAndMakeVisible(NoteBox);
    addAndMakeVisible(KeyBox);
    addAndMakeVisible(LoopEndBox);
    addAndMakeVisible(NotationBox);
}

LooperComponent::~LooperComponent()
{
}

void LooperComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10, 2);

}

void LooperComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    
    const auto NoteboxWidth = getLocalBounds().getWidth()-30;
    const auto TextBoxHeight = getLocalBounds().getHeight()/6;
    KeyBox.setBounds(0, 0, 30, TextBoxHeight);
    NoteBox.setBounds(30, 0, NoteboxWidth, TextBoxHeight);
    LoopEndBox.setBounds(0, TextBoxHeight, 100, TextBoxHeight);
    NotationBox.setBounds(0, TextBoxHeight*2, 40, TextBoxHeight);
    
}

void LooperComponent::NoteBoxChanged()
{
    juce::Logger::writeToLog("CHANGED NOTEBOX");
    looperdata->NoteBoxChanged(NoteBox.getText(),KeyBox.getText());
    //looperdata->NoteBoxChanged(NoteBox.getText());
}

void LooperComponent::LoopEndBoxChanged(juce::AudioProcessorValueTreeState& apvts)
{
    juce::Logger::writeToLog("CHANGED NOTEBOX");
    looperdata->LoopEndBoxChanged(LoopEndBox.getText(),apvts);
}

void LooperComponent::NotationBoxChanged(juce::AudioProcessorValueTreeState& apvts)
{
    juce::Logger::writeToLog("CHANGED NotationBox");

    looperdata->NotationBoxChanged(*apvts.getRawParameterValue("NOTBOX"));
}

juce::String LoopEndFilter::filterNewText(juce::TextEditor& ed, const juce::String& newInput)
{
    //FILTERING INPUT
    juce::String t(newInput);
    return t;
}
