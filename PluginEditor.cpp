/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MASCAudioProcessorEditor::MASCAudioProcessorEditor (MASCAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), looper(audioProcessor.midiProcessor.looperData,audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(looper);
}

MASCAudioProcessorEditor::~MASCAudioProcessorEditor()
{
}

//==============================================================================
void MASCAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

   
}

void MASCAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    looper.setBounds(0, 0, getWidth(), getHeight());
}
