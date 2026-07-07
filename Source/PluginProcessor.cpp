#include "PluginProcessor.h"
#include "PluginEditor.h"

VintageDigitalVerbAudioProcessor::VintageDigitalVerbAudioProcessor()
    : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", Parameters::createParameterLayout())
{
}

void VintageDigitalVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    reverb.prepare (sampleRate, samplesPerBlock, juce::jmax (1, getTotalNumOutputChannels()));
}

void VintageDigitalVerbAudioProcessor::releaseResources()
{
    reverb.reset();
}

bool VintageDigitalVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& input = layouts.getMainInputChannelSet();
    const auto& output = layouts.getMainOutputChannelSet();

    if (input != output)
        return false;

    return output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo();
}

void VintageDigitalVerbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    for (auto channel = getTotalNumInputChannels(); channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear (channel, 0, buffer.getNumSamples());

    reverb.process (buffer, getSettings());
}

juce::AudioProcessorEditor* VintageDigitalVerbAudioProcessor::createEditor()
{
    return new VintageDigitalVerbAudioProcessorEditor (*this);
}

void VintageDigitalVerbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void VintageDigitalVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xmlState = getXmlFromBinary (data, sizeInBytes))
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

ReverbSettings VintageDigitalVerbAudioProcessor::getSettings() const
{
    ReverbSettings settings;

    settings.mix = apvts.getRawParameterValue (Parameters::mix)->load();
    settings.predelayMs = apvts.getRawParameterValue (Parameters::predelay)->load();
    settings.decay = apvts.getRawParameterValue (Parameters::decay)->load();
    settings.size = apvts.getRawParameterValue (Parameters::size)->load();
    settings.damping = apvts.getRawParameterValue (Parameters::damping)->load();
    settings.lowCutHz = apvts.getRawParameterValue (Parameters::lowCut)->load();
    settings.highCutHz = apvts.getRawParameterValue (Parameters::highCut)->load();
    settings.modDepth = apvts.getRawParameterValue (Parameters::modDepth)->load();
    settings.modRateHz = apvts.getRawParameterValue (Parameters::modRate)->load();
    settings.width = apvts.getRawParameterValue (Parameters::width)->load();
    settings.outputGainDb = apvts.getRawParameterValue (Parameters::outputGain)->load();
    settings.mode = juce::roundToInt (apvts.getRawParameterValue (Parameters::mode)->load());
    settings.freeze = apvts.getRawParameterValue (Parameters::freeze)->load() > 0.5f;

    return settings;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VintageDigitalVerbAudioProcessor();
}
