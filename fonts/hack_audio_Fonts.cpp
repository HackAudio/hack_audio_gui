#include "hack_audio_FontData.cpp"

const juce::Typeface::Ptr ptr_NowRegular     = juce::Typeface::createSystemTypefaceFor(NowRegular_otf,     NowRegular_otfSize);
const juce::Typeface::Ptr ptr_BaumansRegular = juce::Typeface::createSystemTypefaceFor(BaumansRegular_ttf, BaumansRegular_ttfSize);

const juce::Font HackAudio::Fonts::Now     (ptr_NowRegular);
const juce::Font HackAudio::Fonts::Baumans (ptr_BaumansRegular);
