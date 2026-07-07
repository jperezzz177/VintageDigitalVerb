#include "PluginEditor.h"
#include "Parameters/ParameterIDs.h"

namespace
{
    constexpr auto backgroundTop = 0xff070b10;
    constexpr auto backgroundBottom = 0xff121b22;
    constexpr auto panel = 0xff111820;
    constexpr auto panelLight = 0xff1a242b;
    constexpr auto stroke = 0xff33424a;
    constexpr auto cream = 0xffeadfc8;
    constexpr auto amber = 0xffffbc63;
    constexpr auto teal = 0xff69d0c5;

    juce::String displayPresetName (const juce::String& name)
    {
        return name.toUpperCase();
    }
}

VintageDigitalVerbAudioProcessorEditor::VintageDigitalVerbAudioProcessorEditor (VintageDigitalVerbAudioProcessor& audioProcessor)
    : AudioProcessorEditor (&audioProcessor),
      processorRef (audioProcessor)
{
    setLookAndFeel (&lookAndFeel);
    setSize (920, 580);

    modeBox.addItemList ({ "Room", "Plate", "Hall", "Vintage Hall", "Chamber" }, 1);
    modeBox.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (modeBox);
    modeAttachment = std::make_unique<ComboAttachment> (processorRef.apvts, Parameters::mode, modeBox);
    modeBox.onChange = [this] { updatePresetDisplay(); };

    modeLabel.setText ("PRESET", juce::dontSendNotification);
    modeLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (modeLabel);

    presetNameLabel.setJustificationType (juce::Justification::centred);
    presetNameLabel.setColour (juce::Label::textColourId, juce::Colour (cream));
    presetNameLabel.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (presetNameLabel);

    previousPresetButton.setButtonText ("<");
    previousPresetButton.onClick = [this] { stepPreset (-1); };
    addAndMakeVisible (previousPresetButton);

    nextPresetButton.setButtonText (">");
    nextPresetButton.onClick = [this] { stepPreset (1); };
    addAndMakeVisible (nextPresetButton);

    freezeButton.setButtonText ("FREEZE");
    addAndMakeVisible (freezeButton);
    freezeAttachment = std::make_unique<ButtonAttachment> (processorRef.apvts, Parameters::freeze, freezeButton);

    updatePresetDisplay();
    startTimerHz (12);
}

VintageDigitalVerbAudioProcessorEditor::~VintageDigitalVerbAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void VintageDigitalVerbAudioProcessorEditor::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient gradient (juce::Colour (backgroundTop), 0.0f, 0.0f,
                                   juce::Colour (backgroundBottom), bounds.getWidth(), bounds.getHeight(), false);
    g.setGradientFill (gradient);
    g.fillAll();

    for (auto y = 82.0f; y < bounds.getHeight(); y += 20.0f)
    {
        g.setColour (juce::Colour (teal).withAlpha (0.025f));
        g.drawHorizontalLine (static_cast<int> (y), 22.0f, bounds.getWidth() - 22.0f);
    }

    auto shell = bounds.reduced (1.0f);
    g.setColour (juce::Colours::black.withAlpha (0.30f));
    g.fillRoundedRectangle (shell.translated (0.0f, 3.0f), 10.0f);

    juce::ColourGradient shellGradient (juce::Colour (0xff172027), shell.getX(), shell.getY(),
                                        juce::Colour (0xff070b10), shell.getRight(), shell.getBottom(), false);
    g.setGradientFill (shellGradient);
    g.fillRoundedRectangle (shell, 10.0f);

    g.setColour (juce::Colour (stroke));
    g.drawRoundedRectangle (shell, 10.0f, 1.0f);

    auto topBar = getLocalBounds().removeFromTop (56).toFloat();
    g.setColour (juce::Colours::black.withAlpha (0.22f));
    g.fillRect (topBar);
    g.setColour (juce::Colour (stroke).withAlpha (0.65f));
    g.drawHorizontalLine (56, 0.0f, bounds.getWidth());

    g.setFont (juce::FontOptions (16.5f, juce::Font::bold));
    auto headerText = juce::AttributedString {};
    headerText.setJustification (juce::Justification::centred);
    headerText.append ("DaCartel AudioPlugins", juce::FontOptions (16.5f, juce::Font::bold), juce::Colour (cream));
    headerText.append (juce::String::fromUTF8 ("  \xE2\x80\xA2  "),
                       juce::FontOptions (16.5f, juce::Font::bold),
                       juce::Colour (teal).withAlpha (0.72f));
    headerText.append ("Vintage Digital Verb", juce::FontOptions (16.5f, juce::Font::bold), juce::Colour (teal).withAlpha (0.86f));
    headerText.draw (g, topBar.reduced (24.0f, 0.0f));

    g.setColour (juce::Colour (teal).withAlpha (0.70f));
    g.fillEllipse (bounds.getRight() - 46.0f, 25.0f, 4.0f, 4.0f);
    g.fillEllipse (bounds.getRight() - 34.0f, 25.0f, 4.0f, 4.0f);
    g.fillEllipse (bounds.getRight() - 22.0f, 25.0f, 4.0f, 4.0f);

    auto title = juce::Rectangle<float> (40.0f, 94.0f, bounds.getWidth() - 80.0f, 88.0f);
    g.setColour (juce::Colour (amber));
    g.setFont (juce::FontOptions (30.0f, juce::Font::plain));
    g.drawFittedText ("V I N T A G E   D I G I T A L   V E R B",
                      title.removeFromTop (42.0f).toNearestInt(), juce::Justification::centred, 1);

    auto subtitle = title.removeFromTop (28.0f);
    g.setColour (juce::Colour (teal).withAlpha (0.82f));
    g.setFont (juce::FontOptions (13.5f, juce::Font::bold));
    g.drawText ("PRESET REVERB PROCESSOR", subtitle, juce::Justification::centred);

    g.setColour (juce::Colour (teal).withAlpha (0.30f));
    g.drawLine (bounds.getCentreX() - 172.0f, 158.0f, bounds.getCentreX() - 118.0f, 158.0f, 1.0f);
    g.drawLine (bounds.getCentreX() + 118.0f, 158.0f, bounds.getCentreX() + 172.0f, 158.0f, 1.0f);

    auto presetPanel = juce::Rectangle<float> (90.0f, 208.0f, bounds.getWidth() - 180.0f, 194.0f);
    drawPanel (g, presetPanel, juce::Colour (amber));

    g.setColour (juce::Colour (teal).withAlpha (0.16f));
    g.fillRoundedRectangle (presetPanel.reduced (8.0f), 13.0f);
    g.setColour (juce::Colour (teal).withAlpha (0.82f));
    g.setFont (juce::FontOptions (13.0f, juce::Font::bold));
    g.drawText ("CURRENT PRESET", presetPanel.withY (244.0f).withHeight (24.0f), juce::Justification::centred);

    g.setColour (juce::Colour (amber).withAlpha (0.38f));
    g.fillRoundedRectangle (juce::Rectangle<float> (bounds.getCentreX() - 24.0f, presetPanel.getY() - 2.0f, 48.0f, 4.0f), 2.0f);

    g.setColour (juce::Colour (teal).withAlpha (0.42f));
    g.fillEllipse (bounds.getCentreX() - 14.0f, presetPanel.getBottom() - 38.0f, 5.0f, 5.0f);
    g.fillEllipse (bounds.getCentreX() - 2.5f, presetPanel.getBottom() - 38.0f, 5.0f, 5.0f);
    g.fillEllipse (bounds.getCentreX() + 9.0f, presetPanel.getBottom() - 38.0f, 5.0f, 5.0f);

    auto selectorPanel = juce::Rectangle<float> (190.0f, 434.0f, bounds.getWidth() - 380.0f, 64.0f);
    drawPanel (g, selectorPanel, juce::Colour (teal));

    g.setColour (juce::Colour (teal).withAlpha (0.40f));
    g.drawVerticalLine (static_cast<int> (selectorPanel.getX() + 118.0f), selectorPanel.getY() + 14.0f, selectorPanel.getBottom() - 14.0f);
}

void VintageDigitalVerbAudioProcessorEditor::resized()
{
    const auto bounds = getLocalBounds().toFloat();
    auto presetPanel = juce::Rectangle<float> (90.0f, 208.0f, bounds.getWidth() - 180.0f, 194.0f);

    previousPresetButton.setBounds (presetPanel.withWidth (70.0f).withTrimmedLeft (20.0f).withY (presetPanel.getCentreY() - 25.0f).withHeight (50.0f).toNearestInt());
    nextPresetButton.setBounds (presetPanel.withX (presetPanel.getRight() - 70.0f).withWidth (50.0f).withY (presetPanel.getCentreY() - 25.0f).withHeight (50.0f).toNearestInt());
    presetNameLabel.setBounds (presetPanel.reduced (105.0f, 62.0f).toNearestInt());

    auto selectorPanel = juce::Rectangle<float> (190.0f, 434.0f, bounds.getWidth() - 380.0f, 64.0f);
    modeLabel.setBounds (selectorPanel.withWidth (100.0f).reduced (28.0f, 20.0f).toNearestInt());
    modeBox.setBounds (selectorPanel.withX (selectorPanel.getX() + 134.0f).withWidth (selectorPanel.getWidth() - 268.0f).reduced (0.0f, 13.0f).toNearestInt());
    freezeButton.setBounds (selectorPanel.withX (selectorPanel.getRight() - 118.0f).withWidth (94.0f).reduced (0.0f, 13.0f).toNearestInt());
}

void VintageDigitalVerbAudioProcessorEditor::drawPanel (juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour accent) const
{
    g.setColour (juce::Colours::black.withAlpha (0.22f));
    g.fillRoundedRectangle (bounds.translated (0.0f, 2.0f), 7.0f);

    g.setColour (juce::Colour (panel));
    g.fillRoundedRectangle (bounds, 7.0f);

    g.setColour (juce::Colour (panelLight).withAlpha (0.55f));
    g.fillRoundedRectangle (bounds.reduced (1.0f), 6.0f);

    g.setColour (juce::Colour (stroke));
    g.drawRoundedRectangle (bounds, 7.0f, 1.0f);

    g.setColour (accent.withAlpha (0.32f));
    g.drawLine (bounds.getX() + 12.0f, bounds.getY() + 1.0f,
                bounds.getRight() - 12.0f, bounds.getY() + 1.0f, 1.0f);
}

void VintageDigitalVerbAudioProcessorEditor::updatePresetDisplay()
{
    auto text = modeBox.getText();

    if (text.isEmpty())
        if (auto* parameter = dynamic_cast<juce::AudioParameterChoice*> (processorRef.apvts.getParameter (Parameters::mode)))
            text = parameter->getCurrentChoiceName();

    presetNameLabel.setFont (juce::FontOptions (34.0f, juce::Font::plain));
    presetNameLabel.setText (displayPresetName (text), juce::dontSendNotification);
}

void VintageDigitalVerbAudioProcessorEditor::stepPreset (int delta)
{
    const auto itemCount = modeBox.getNumItems();

    if (itemCount <= 0)
        return;

    auto selectedIndex = modeBox.getSelectedItemIndex();

    if (selectedIndex < 0)
        selectedIndex = 0;

    const auto nextIndex = (selectedIndex + delta + itemCount) % itemCount;
    modeBox.setSelectedItemIndex (nextIndex, juce::sendNotificationSync);
    updatePresetDisplay();
}

void VintageDigitalVerbAudioProcessorEditor::timerCallback()
{
    updatePresetDisplay();
}
