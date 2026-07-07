#include "VintageReverb.h"

void VintageReverb::prepare (double sampleRate, int maximumBlockSize, int channels)
{
    currentSampleRate = sampleRate;
    numChannels = juce::jmax (1, channels);

    const juce::dsp::ProcessSpec spec { sampleRate,
                                        static_cast<juce::uint32> (maximumBlockSize),
                                        static_cast<juce::uint32> (numChannels) };

    predelay.prepare (spec);
    predelay.setMaximumDelayInSamples (static_cast<int> (sampleRate * 0.35));
    predelay.setDelay (0.0f);

    reverb.prepare (spec);
    chorus.prepare (spec);
    lowCut.prepare (spec);
    highCut.prepare (spec);
    outputGain.prepare (spec);

    mixSmoothed.reset (sampleRate, 0.03);
    ensureScratchSize (numChannels, maximumBlockSize);
    reset();
}

void VintageReverb::reset()
{
    predelay.reset();
    reverb.reset();
    chorus.reset();
    lowCut.reset();
    highCut.reset();
    outputGain.reset();
}

void VintageReverb::process (juce::AudioBuffer<float>& buffer, const ReverbSettings& settings)
{
    if (buffer.getNumSamples() == 0)
        return;

    updateSettings (settings);
    ensureScratchSize (buffer.getNumChannels(), buffer.getNumSamples());

    dryBuffer.makeCopyOf (buffer, true);
    wetBuffer.makeCopyOf (buffer, true);

    juce::dsp::AudioBlock<float> wetBlock { wetBuffer };
    juce::dsp::ProcessContextReplacing<float> wetContext { wetBlock };

    predelay.process (wetContext);
    reverb.process (wetContext);
    lowCut.process (wetContext);
    highCut.process (wetContext);
    chorus.process (wetContext);

    auto mix = mixSmoothed.getCurrentValue();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        mix = mixSmoothed.getNextValue();

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            const auto dry = dryBuffer.getSample (channel, sample);
            const auto wet = wetBuffer.getSample (channel, sample);
            buffer.setSample (channel, sample, dry + (wet - dry) * mix);
        }
    }

    juce::dsp::AudioBlock<float> outBlock { buffer };
    juce::dsp::ProcessContextReplacing<float> outContext { outBlock };
    outputGain.process (outContext);
}

void VintageReverb::updateSettings (const ReverbSettings& settings)
{
    mixSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, settings.mix));

    const auto mode = juce::jlimit (0, 4, settings.mode);
    const float sizeBias[]  { 0.45f, 0.70f, 0.90f, 0.96f, 0.62f };
    const float dampBias[]  { 0.35f, 0.42f, 0.54f, 0.62f, 0.50f };
    const float decayBias[] { 0.55f, 0.72f, 0.86f, 0.92f, 0.68f };

    juce::Reverb::Parameters params;
    params.roomSize = juce::jlimit (0.0f, 1.0f, settings.size * 0.72f + sizeBias[mode] * 0.28f);
    params.damping = juce::jlimit (0.0f, 1.0f, settings.damping * 0.72f + dampBias[mode] * 0.28f);
    params.wetLevel = 1.0f;
    params.dryLevel = 0.0f;
    params.width = juce::jlimit (0.0f, 1.0f, settings.width);
    params.freezeMode = settings.freeze ? 1.0f : 0.0f;

    const auto decayShaper = settings.decay * 0.75f + decayBias[mode] * 0.25f;
    params.roomSize = juce::jlimit (0.0f, 1.0f, params.roomSize * (0.72f + decayShaper * 0.28f));
    reverb.setParameters (params);

    predelay.setDelay (static_cast<float> (currentSampleRate * settings.predelayMs / 1000.0));

    *lowCut.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (currentSampleRate,
                                                                        juce::jlimit (20.0f, 18000.0f, settings.lowCutHz),
                                                                        0.707f);

    *highCut.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (currentSampleRate,
                                                                         juce::jlimit (250.0f, 20000.0f, settings.highCutHz),
                                                                         0.707f);

    chorus.setRate (juce::jlimit (0.05f, 3.0f, settings.modRateHz));
    chorus.setDepth (juce::jlimit (0.0f, 1.0f, settings.modDepth) * 0.42f);
    chorus.setCentreDelay (7.0f + settings.size * 8.0f);
    chorus.setFeedback (0.06f);
    chorus.setMix (juce::jlimit (0.0f, 0.45f, settings.modDepth * 0.35f));

    outputGain.setGainDecibels (settings.outputGainDb);
}

void VintageReverb::ensureScratchSize (int channels, int samples)
{
    if (dryBuffer.getNumChannels() != channels || dryBuffer.getNumSamples() < samples)
    {
        dryBuffer.setSize (channels, samples, false, false, true);
        wetBuffer.setSize (channels, samples, false, false, true);
    }
}
