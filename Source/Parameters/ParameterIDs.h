#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace Parameters
{
    inline constexpr auto mix        = "mix";
    inline constexpr auto predelay   = "predelay";
    inline constexpr auto decay      = "decay";
    inline constexpr auto size       = "size";
    inline constexpr auto damping    = "damping";
    inline constexpr auto lowCut     = "lowCut";
    inline constexpr auto highCut    = "highCut";
    inline constexpr auto modDepth   = "modDepth";
    inline constexpr auto modRate    = "modRate";
    inline constexpr auto width      = "width";
    inline constexpr auto outputGain = "outputGain";
    inline constexpr auto mode       = "mode";
    inline constexpr auto freeze     = "freeze";

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
}
