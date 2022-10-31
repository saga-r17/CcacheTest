/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 11 Oct 2022 11:06:12am
    Author:  Gustavo

  ==============================================================================
*/

#include "MidiProcessor.h"
#include "PluginProcessor.h"


void MidiProcessor::process(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midiMessages, juce::AudioProcessorValueTreeState& apvts)
{       
        //APVTS
        const int notation = *apvts.getRawParameterValue("NOTBOX");
        const int endBar = *apvts.getRawParameterValue("ENDBAR");
        const int end16n = *apvts.getRawParameterValue("END16");

        // -------------------------------------------------------------------------
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = owner->getTotalNumInputChannels();
        auto totalNumOutputChannels = owner->getTotalNumOutputChannels();

        // -------------------------------------------------------------------------
        for (auto i = 0; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, buffer.getNumSamples());

        // -------------------------------------------------------------------------
        const juce::ScopedTryLock myScopedLock(processLock);

        ownTransportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));

        if (myScopedLock.isLocked())
        {

            auto thePositionInfo = owner->getPlayHead()->getPosition();

            //DEBUGGER
                // The MIDI sequence is played only when the transport is active
                // TODO: point of transport override. add ignore transport and use own transport
                if (useOwnTransportInstead) {
                    //if (ownIsPlaying) {
                    //    // playing
                    //    const juce::MidiMessageSequence* theSequence = looperData.midiSequence.get();

                    //    ownOffsetTime = fmod(ownStartPpq, traverseEndPpq);
                    //    ownStartPpq = (double)*thePositionInfo->getTimeInSeconds() - ownOffsetPpq;
                    //    ownElapsedPpq += 0;
                    //}
                    //else {
                    //    // paused or maybe stop
                    //    // add offset by frame time!
                    //    ownOffsetPpq = (double)*thePositionInfo->getPpqInSeconds();
                    //}

                }
                else {
                    //If DAW is playing:
                    if (thePositionInfo->getIsPlaying())
                    {
                        const juce::MidiMessageSequence* theSequence = looperData.midiSequence.get();

                        //This should be in LooperDATA for performance!!!!
                        juce::AudioPlayHead::TimeSignature timeSig = *thePositionInfo->getTimeSignature();


                        traverseEndPpq = (end16n/4)+(endBar* (4 / timeSig.denominator) * timeSig.numerator);

                        auto startPpq = (double)*thePositionInfo->getPpqPosition();

                        //loop to ppq
                        double loopOffset = fmod(startPpq, traverseEndPpq); // do repull here instead?
                        loop = startPpq - loopOffset;
                        auto samplestartPpq = startPpq - (doLoop ? loop : 0.0);

                        if (lastSampleStartPpq > samplestartPpq)
                        {
                            samplestartPpq = 0.0; // set to 0 if new loop starts
                            tellLoopPull = false;
                        }

                        lastSampleStartPpq = samplestartPpq;
                        auto sampleEndPpq = lastSampleStartPpq + (*thePositionInfo->getBpm() * buffer.getNumSamples() / owner->getSampleRate() /60 );
                        auto sampleLength = (*thePositionInfo->getBpm() * 1.0 / owner->getSampleRate()/60); // sample length is here!

                        // If the transport bar position has been moved by the user or because of looping
                        //if (std::abs(startTime - nextStartTime) > sampleLength && nextStartPpq > 0.0)
                        if (std::abs(samplestartPpq - nextStartPpq) > sampleLength && nextStartPpq > 0.0)
                        {sendAllNotesOff(midiMessages);
                        }

                        //nextStartPpq = endPpq;
                        nextStartPpq = sampleEndPpq;

                        // If the MIDI file doesn't contain any event anymore
                        if (isPlayingSomething && samplestartPpq >= traverseEndPpq)
                        {sendAllNotesOff(midiMessages);
                        }
                        else
                        {
                            int curTranspose = transpose, lastTranspose = transpose; 
                                // copy playback from bellow
                                for (auto j = 0; j < theSequence->getNumEvents(); j++) {
                                    juce::MidiMessageSequence::MidiEventHolder* event = theSequence->getEventPointer(j);

                                    if (event->message.getTimeStamp() / pow(2, notation) >= samplestartPpq && event->message.getTimeStamp() / pow(2, notation) < sampleEndPpq)
                                    {
                                        auto samplePosition = juce::roundToInt(((event->message.getTimeStamp()/pow(2,notation)) - samplestartPpq) * (owner->getSampleRate()*60 /(*thePositionInfo->getBpm())));
                                        midiMessages.addEvent(event->message, samplePosition);

                                        
                                        if (fmod(startPpq, traverseEndPpq) > fmod(sampleEndPpq, traverseEndPpq))
                                        {
                                            juce::MidiMessageSequence::MidiEventHolder event2 = *theSequence->getEventPointer(0);
                                            auto samplePosition = juce::roundToInt(((event2.message.getTimeStamp()) / pow(2, notation)) * (owner->getSampleRate() * 60 / (*thePositionInfo->getBpm())));
                                            midiMessages.addEvent(event2.message, samplePosition);
                                        }

                                        isPlayingSomething = true;
                                    }
                                }//for
                            
                        }//else

                    }//if
                    else
                    {
                        // If the transport isn't active anymore
                        sendAllNotesOff(midiMessages);
                        if (isPlayingSomething)
                        { sendAllNotesOff(midiMessages);
                        }
                    }
                }//else
            
        }

    
    
}

void MidiProcessor::sendAllNotesOff(juce::MidiBuffer& midiMessages)
{
    midiMessages.clear();
    for (auto i = 1; i <= 16; i++)
    {
        midiMessages.addEvent(juce::MidiMessage::allNotesOff(i), 0);
        midiMessages.addEvent(juce::MidiMessage::allSoundOff(i), 0);
        midiMessages.addEvent(juce::MidiMessage::allControllersOff(i), 0);
    }

    isPlayingSomething = false;
    tellWorkaroundFirst = false;
    tellLoopPull = false;
}

