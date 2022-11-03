/*
  ==============================================================================

    LooperData.cpp
    Created: 11 Oct 2022 10:16:16am
    Author:  Gustavo

  ==============================================================================
*/

#include "LooperData.h"

LooperData::LooperData()
{
    //Initiate MidiSequence
    midiSequence = std::make_unique<juce::MidiMessageSequence>();
    //Temp init of sequence
    auto messageOn = juce::MidiMessage::noteOn(1, 60, (juce::uint8)100);
    auto messageOff = juce::MidiMessage::noteOff(1, 60, (juce::uint8)100);
    midiSequence->addEvent(messageOn);
    midiSequence->addEvent(messageOff);
}




void LooperData::NoteBoxChanged(juce::String text,juce::String keyText)
{
    //Parse to Midi Sequence
    midiSequence->clear();
    double temptime = 0;
    int keyNumber = KeyNameToNumber(keyText);
    for (juce::String::CharPointerType character(text.getCharPointer()); character != character.findTerminatingNull(); ++character)
    {


        //TODO:Optimize, parse strings to int first
        if (juce::String(character, 1) == juce::String("1")) {
            //PARSE KEYNUMBER FROM KEYTEXT
            auto messageOn = juce::MidiMessage::noteOn(1, keyNumber, (juce::uint8)100);
            messageOn.setTimeStamp(temptime);
            midiSequence->addEvent(messageOn);
            auto messageOff = juce::MidiMessage::noteOff(1, keyNumber, (juce::uint8)100);
            messageOff.setTimeStamp(++temptime);
            midiSequence->addEvent(messageOff);

        }
        else {
            ++temptime;
            //auto message = juce::MidiMessage::noteOn(1, 20, (juce::uint8)100);
            //juce::Logger::writeToLog("silence");
            //midiSequence->addEvent(message);


        }
        //midiSequence->addTimeToMessages
        auto endLoopMessage = juce::MidiMessage::allNotesOff(1);
        endLoopMessage.setTimeStamp(temptime);
        midiSequence->addEvent(endLoopMessage);
    }
    

}

void LooperData::LoopEndBoxChanged(juce::String text, juce::AudioProcessorValueTreeState& apvts)
{
    juce::StringArray textArray;
    textArray.addTokens(text,juce::String(":"), juce::String());

    int endLoopBars = textArray[0].getIntValue();
    int endLoop16n = (4*textArray[1].getIntValue())+textArray[2].getIntValue();
    
    apvts.getParameter("ENDBAR")->setValueNotifyingHost(apvts.getParameter("ENDBAR")->convertTo0to1(endLoopBars));
    apvts.getParameter("END16")->setValueNotifyingHost(apvts.getParameter("END16")->convertTo0to1(endLoop16n));
}

void LooperData::NotationBoxChanged(const int notation)
{
    //for (auto j = 0; j < midiSequence->getNumEvents(); j++) {
    //    juce::MidiMessageSequence::MidiEventHolder* event = midiSequence->getEventPointer(j);
    //    event->message.setTimeStamp(event->message.getTimeStamp() / pow(2,notation));
    //}
}

int LooperData::KeyNameToNumber(const juce::String& keyName)
{
    static const char* const noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "", "Db", "", "Eb", "", "", "Gb", "", "Ab", "", "Bb" };

    int keyNumber, octave = 0, numPos = keyName.indexOfAnyOf("01234567890-");

    if (numPos == 0)
        keyNumber = keyName.getIntValue();	//apparently already a number!

    else
    {
        if (numPos > 0)
        {
            octave = keyName.substring(numPos).getIntValue() + 1;
        }
        else
        {
            octave = 5;		//default to octave of middle C if none found
            numPos = keyName.length();
        }

        juce::String name(keyName.substring(0, numPos).trim().toUpperCase());

        keyNumber = juce::StringArray(noteNames, 12).indexOf(name) % 12;

        if (keyNumber < 0) {
            DBG("Phoney keyname: " << keyName);
        }
    }

    return keyNumber + octave * 12;
}


