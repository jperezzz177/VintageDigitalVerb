#include "ParameterIDs.h"

namespace
{
    juce::String percentText (float value, int)
    {
        return juce::String (juce::roundToInt (value * 100.0f)) + "%";
    }

    juce::String msText (float value, int)
    {
        return juce::String (value, value < 10.0f ? 1 : 0) + " ms";
    }

    juce::String hzText (float value, int)
    {
        return value >= 1000.0f ? juce::String (value / 1000.0f, 1) + " kHz"
                                : juce::String (juce::roundToInt (value)) + " Hz";
    }

    juce::String dbText (float value, int)
    {
        return juce::String (value, 1) + " dB";
    }
}

namespace Parameters
{
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { mix, 1 }, "Mix",
            juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.35f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { predelay, 1 }, "Pre-delay",
            juce::NormalisableRange<float> { 0.0f, 250.0f, 0.1f }, 24.0f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (msText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { decay, 1 }, "Decay",
            juce::NormalisableRange<float> { 0.15f, 0.98f, 0.001f }, 0.72f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { size, 1 }, "Size",
            juce::NormalisableRange<float> { 0.05f, 1.0f, 0.001f }, 0.78f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { damping, 1 }, "Damping",
            juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.48f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { lowCut, 1 }, "Low Cut",
            juce::NormalisableRange<float> { 20.0f, 1000.0f, 0.1f, 0.35f }, 120.0f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (hzText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { highCut, 1 }, "High Cut",
            juce::NormalisableRange<float> { 1000.0f, 20000.0f, 1.0f, 0.45f }, 9200.0f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (hzText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { modDepth, 1 }, "Mod Depth",
            juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.26f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { modRate, 1 }, "Mod Rate",
            juce::NormalisableRange<float> { 0.05f, 3.0f, 0.001f, 0.45f }, 0.42f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (float value, int) { return juce::String (value, 2) + " Hz"; })));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { width, 1 }, "Width",
            juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.86f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (percentText)));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { outputGain, 1 }, "Output Gain",
            juce::NormalisableRange<float> { -24.0f, 12.0f, 0.01f }, 0.0f,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction (dbText)));

        params.push_back (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID { mode, 1 }, "Mode",
            juce::StringArray { "Room", "Plate", "Hall", "Vintage Hall", "Chamber" }, 3));

        params.push_back (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID { freeze, 1 }, "Freeze", false));

        return { params.begin(), params.end() };
    }
}
