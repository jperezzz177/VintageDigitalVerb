#include "VintageLookAndFeel.h"

namespace
{
    constexpr auto panelDark = 0xff111820;
    constexpr auto panelMid = 0xff17222b;
    constexpr auto outline = 0xff334250;
    constexpr auto teal = 0xff69d0c5;
    constexpr auto amber = 0xffffbc63;
    constexpr auto cream = 0xffeadfc8;
}

VintageLookAndFeel::VintageLookAndFeel()
{
    setColour (juce::Slider::thumbColourId, juce::Colour (cream));
    setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (amber));
    setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (outline));
    setColour (juce::Slider::textBoxTextColourId, juce::Colour (cream));
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff0b1015));
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x00334250));
    setColour (juce::Label::textColourId, juce::Colour (0xffd8cfbf));
    setColour (juce::ComboBox::backgroundColourId, juce::Colour (panelDark));
    setColour (juce::ComboBox::outlineColourId, juce::Colour (outline));
    setColour (juce::ComboBox::textColourId, juce::Colour (cream));
    setColour (juce::TextButton::buttonColourId, juce::Colour (panelMid));
    setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff1c4d4b));
    setColour (juce::ToggleButton::textColourId, juce::Colour (cream));
}

void VintageLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                           juce::Slider&)
{
    const auto bounds = juce::Rectangle<float> (static_cast<float> (x), static_cast<float> (y),
                                                static_cast<float> (width), static_cast<float> (height)).reduced (9.0f);
    const auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto centre = bounds.getCentre();
    const auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const auto arcRadius = radius - 6.0f;

    g.setColour (juce::Colours::black.withAlpha (0.26f));
    g.fillEllipse (bounds.translated (0.0f, 3.0f).expanded (1.0f));

    juce::ColourGradient knobGradient (juce::Colour (0xff25313a), bounds.getCentreX(), bounds.getY(),
                                       juce::Colour (0xff0c1117), bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill (knobGradient);
    g.fillEllipse (bounds);

    g.setColour (juce::Colour (0xff46505a).withAlpha (0.75f));
    g.drawEllipse (bounds, 1.2f);

    g.setColour (juce::Colour (0xff020406).withAlpha (0.85f));
    g.drawEllipse (bounds.reduced (6.5f), 1.0f);

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                                 rotaryStartAngle, rotaryEndAngle, true);
    g.setColour (juce::Colour (0xff31404b));
    g.strokePath (backgroundArc, juce::PathStrokeType (3.8f, juce::PathStrokeType::curved));

    juce::Path valueArc;
    valueArc.addCentredArc (centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                            rotaryStartAngle, angle, true);
    g.setColour (juce::Colour (amber).withAlpha (0.18f));
    g.strokePath (valueArc, juce::PathStrokeType (8.0f, juce::PathStrokeType::curved));

    g.setColour (juce::Colour (amber));
    g.strokePath (valueArc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved));

    for (auto i = 0; i < 11; ++i)
    {
        const auto tickAngle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * static_cast<float> (i) / 10.0f;
        const auto tickAlpha = i % 5 == 0 ? 0.45f : 0.22f;
        const auto inner = juce::Point<float> (centre.x + std::cos (tickAngle - juce::MathConstants<float>::halfPi) * (radius + 1.0f),
                                               centre.y + std::sin (tickAngle - juce::MathConstants<float>::halfPi) * (radius + 1.0f));
        const auto outer = juce::Point<float> (centre.x + std::cos (tickAngle - juce::MathConstants<float>::halfPi) * (radius + 5.0f),
                                               centre.y + std::sin (tickAngle - juce::MathConstants<float>::halfPi) * (radius + 5.0f));

        g.setColour (juce::Colour (cream).withAlpha (tickAlpha));
        g.drawLine ({ inner, outer }, i % 5 == 0 ? 1.1f : 0.8f);
    }

    juce::Path pointer;
    pointer.addRoundedRectangle (-1.6f, -arcRadius + 6.0f, 3.2f, 15.0f, 1.6f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));
    g.setColour (juce::Colour (cream));
    g.fillPath (pointer);

    g.setColour (juce::Colour (teal).withAlpha (0.16f));
    g.fillEllipse (bounds.reduced (radius * 0.62f));
}

void VintageLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                                       int buttonX, int buttonY, int buttonW, int buttonH,
                                       juce::ComboBox& comboBox)
{
    juce::ignoreUnused (buttonX, buttonY, buttonW, buttonH);

    auto bounds = juce::Rectangle<float> (0.0f, 0.0f, static_cast<float> (width), static_cast<float> (height)).reduced (0.5f);
    g.setColour (comboBox.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (bounds, 5.0f);

    g.setColour (juce::Colour (teal).withAlpha (isButtonDown ? 0.25f : 0.12f));
    g.fillRoundedRectangle (bounds.reduced (2.0f), 4.0f);

    g.setColour (comboBox.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (bounds, 5.0f, 1.0f);

    juce::Path arrow;
    const auto arrowCentre = juce::Point<float> (bounds.getRight() - 18.0f, bounds.getCentreY() + 1.0f);
    arrow.addTriangle (arrowCentre.x - 5.0f, arrowCentre.y - 3.0f,
                       arrowCentre.x + 5.0f, arrowCentre.y - 3.0f,
                       arrowCentre.x, arrowCentre.y + 4.0f);
    g.setColour (juce::Colour (amber));
    g.fillPath (arrow);
}

void VintageLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                               const juce::Colour& backgroundColour,
                                               bool shouldDrawButtonAsHighlighted,
                                               bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    const auto active = button.getToggleState();

    g.setColour (juce::Colours::black.withAlpha (0.20f));
    g.fillRoundedRectangle (bounds.translated (0.0f, 2.0f), 5.0f);

    g.setColour (active ? juce::Colour (teal).withAlpha (0.48f)
                        : backgroundColour.brighter (shouldDrawButtonAsHighlighted ? 0.12f : 0.0f));
    g.fillRoundedRectangle (bounds, 5.0f);

    g.setColour ((active ? juce::Colour (teal) : juce::Colour (outline)).withAlpha (shouldDrawButtonAsDown ? 1.0f : 0.75f));
    g.drawRoundedRectangle (bounds, 5.0f, 1.0f);
}

void VintageLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    drawButtonBackground (g, button, findColour (juce::TextButton::buttonColourId),
                          shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto bounds = button.getLocalBounds().toFloat().reduced (12.0f, 0.0f);
    g.setColour (button.getToggleState() ? juce::Colour (0xffc8fff5) : juce::Colour (cream).withAlpha (0.74f));
    g.setFont (juce::FontOptions (13.0f, juce::Font::bold));
    g.drawFittedText (button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, 1);
}

juce::Font VintageLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::FontOptions (12.0f, juce::Font::plain).withHeight (12.0f);
}

juce::Font VintageLookAndFeel::getComboBoxFont (juce::ComboBox&)
{
    return juce::FontOptions (14.0f, juce::Font::bold);
}
