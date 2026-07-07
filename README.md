# Vintage Digital Verb

A first-milestone JUCE audio plugin project for Xcode. The plugin is an original vintage-inspired algorithmic reverb with warm modulation, smooth filtering, pre-delay, width, freeze, and simple mode shaping. JUCE is vendored in `Vendor/JUCE`, so the project can build from this folder without relying on a global JUCE install.

The references in the brief are used only as high-level sonic and visual inspiration. This project does not copy brand names, logos, protected UI, preset names, or proprietary algorithms.

## Open in Xcode

1. Open `Builds/MacOSX/VintageDigitalVerb.xcodeproj`.
2. Choose `VintageDigitalVerb - All` to build AU and VST3 together, or choose the AU/VST3 schemes individually.
3. Use the `Debug` configuration while developing.

If Xcode command line tools are not pointed at full Xcode, build from Terminal with:

```sh
DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer xcodebuild -project Builds/MacOSX/VintageDigitalVerb.xcodeproj -scheme "VintageDigitalVerb - All" -configuration Debug build
```

## Test in JUCE AudioPluginHost

1. Build the plugin in Xcode.
2. Open JUCE AudioPluginHost.
3. Add or scan the built plugin products:
   - `Builds/MacOSX/build/Debug/VintageDigitalVerb.component`
   - `Builds/MacOSX/build/Debug/VintageDigitalVerb.vst3`
4. Create an audio graph with an input, `Vintage Digital Verb`, and an output.
5. Pass audio through it and adjust Mix, Decay, Size, Mod Depth, and Mode first.

## Main Files

- `Source/PluginProcessor.*`: JUCE processor, APVTS state, parameter reads, audio processing entry point.
- `Source/PluginEditor.*`: main plugin UI and parameter attachments.
- `Source/Parameters/*`: parameter IDs and JUCE 8-compatible parameter layout.
- `Source/DSP/VintageReverb.*`: pre-delay, JUCE reverb core, damping filters, wet modulation, width, output gain.
- `Source/GUI/VintageLookAndFeel.*`: minimal dark/vintage-inspired knob drawing and colors.
