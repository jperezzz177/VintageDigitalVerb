#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "GUI/VintageLookAndFeel.h"

class VintageDigitalVerbAudioProcessorEditor final : public juce::AudioProcessorEditor
                                            , private juce::Timer
{
public:
    explicit VintageDigitalVerbAudioProcessorEditor (VintageDigitalVerbAudioProcessor&);
    ~VintageDigitalVerbAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    void drawPanel (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour accent) const;
    void updatePresetDisplay();
    void stepPreset (int delta);
    void timerCallback() override;

    VintageDigitalVerbAudioProcessor& processorRef;
    VintageLookAndFeel lookAndFeel;

    juce::Label presetNameLabel;
    juce::TextButton previousPresetButton { "<" };
    juce::TextButton nextPresetButton { ">" };
    juce::ComboBox modeBox;
    juce::Label modeLabel;
    juce::ToggleButton freezeButton;

    std::unique_ptr<ComboAttachment> modeAttachment;
    std::unique_ptr<ButtonAttachment> freezeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VintageDigitalVerbAudioProcessorEditor)
};
