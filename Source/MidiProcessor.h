/*
  ==============================================================================

    MidiProcessor.h
    Created: 11 Oct 2022 11:01:43am
    Author:  Gustavo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LooperData.h"
class MASCAudioProcessor;

class MidiProcessor {
public:
    MidiProcessor(MASCAudioProcessor* ownerfilter):owner(ownerfilter){};
    void process(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midiMessages, juce::AudioProcessorValueTreeState& apvts);
    void sendAllNotesOff(juce::MidiBuffer& midiMessages);

    double secondsToPpq(double seconds);

    LooperData looperData;

private:
    MASCAudioProcessor* owner;
   // juce::AudioParameterFloat* speed;

    bool isPlayingSomething;                    // Tells if the last audio buffer included some MIDI content to play
    bool useOwnTransportInstead = false;

    bool myOwnIsPlaying = false;                // Own Transport Press Play mode.
    bool doLoop = true;                        // loop auto
    bool doSpacer = true;                       // space after total end?
    juce::AudioPlayHead::CurrentPositionInfo thePositionInfo;

    std::atomic<int> totalNumEvents;
    double ownStartTime;                        // JOELwindows7: start time for own transport
    double ownOffsetTime;                       // JOELwindows7: offset increases by frame time when pauses.
    double traverseEndPpq;        // JOELwindows7: Overall MIDI end time traversing sequence by sequence. who's the highest end time?
    double ownElapsedTime;                      // JOELwindows7: to be ++ every timer callbacks
    double lastSampleStartPpq;                 // JOELwindows7: for position pulling here purpose & loop. see https://forum.juce.com/t/how-to-loop-midi-file/33837/10?u=joelwindows
    double loop;                                // JOELwindows7: loop final calculations
    double nextStartPpq = -1.0;

    int transpose = 0;                          // JOELwindows7: adissu had transposer. it starts from 0 & typically adjustable from -12 to 12.
    bool haveBeenSpaced = false;                // JOELwindows7: tell if the traverse end time have been spaced.
    //bool ownPause = false;                      // JOELwindows7: if own pause true, it'll add spacer by elapsed time every tick.
    bool ownIsPlaying = false;                  // JOELwindows7: how about vice versa? if true, then the music runs.
    bool ownStopPlaying = true;


    bool tellPlayNow = false;
    bool tellStopNow = false;
    bool tellRecordNow = false;
    bool tellRewindNow = false;
    bool tellWorkaroundFirst = false;
    bool tellLoopPull = false;
    bool tellPanic = false; // heart fibrilation
    int tellInfoModes = 0; //which modes? Info, Message log, or Lyric?
    int tempFillOutSetMode = 0;



    juce::AudioTransportSource ownTransportSource; 

    juce::CriticalSection processLock;


};