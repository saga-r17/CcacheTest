
#pragma once
#include <shared_processing_code/shared_processing_code.h>

struct Sync
{
    void prepare(double sampleRate, int maxBlockSize)
    {

        samplesInMinute = sampleRate*60;
        ppqPositions.resize(maxBlockSize);

    }

    double getPPQPerSample() const
    {
        auto samplesPerBeat = samplesInMinute / info.bpm;
        return 1.0 / samplesPerBeat;
    }

    void process(juce::AudioPlayHead* playHead, int numSamples)
    {
        if (playHead != nullptr)
        {
            playHead->getCurrentPosition(info);

        }

        auto ppqPerSample = getPPQPerSample();

        double offset = 0.0;

        for (int sample = 0;sample < numSamples;++sample)
        {
            ppqPositions[sample] = info.ppqPosition + offset;
            offset += ppqPerSample;
        }
        currentPosition.store(info.ppqPosition + offset);
    }


    std::atomic<double> currentPosition{ 0 };
    double samplesInMinute = 0.0;
    juce::AudioPlayHead::CurrentPositionInfo info;
    std::vector<double> ppqPositions;
};
#pragma clang diagnostic pop