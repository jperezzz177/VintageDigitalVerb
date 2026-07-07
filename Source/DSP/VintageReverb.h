#pragma once

#include <juce_dsp/juce_dsp.h>

struct ReverbSettings
{
    float mix = 0.35f;
    float predelayMs = 24.0f;
    float decay = 0.72f;
    float size = 0.78f;
    float damping = 0.48f;
    float lowCutHz = 120.0f;
    float highCutHz = 9200.0f;
    float modDepth = 0.26f;
    float modRateHz = 0.42f;
    float width = 0.86f;
    float outputGainDb = 0.0f;
    int mode = 3;
    bool freeze = false;
};

class VintageReverb
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int channels);
    void reset();
    void process (juce::AudioBuffer<float>& buffer, const ReverbSettings& settings);

private:
    void updateSettings (const ReverbSettings& settings);
    void ensureScratchSize (int channels, int samples);

    double currentSampleRate = 44100.0;
    int numChannels = 2;

    juce::dsp::Reverb reverb;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> predelay { 96000 };
    juce::dsp::Chorus<float> chorus;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowCut;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highCut;
    juce::dsp::Gain<float> outputGain;
    juce::SmoothedValue<float> mixSmoothed { 0.35f };

    juce::AudioBuffer<float> dryBuffer;
    juce::AudioBuffer<float> wetBuffer;
};
