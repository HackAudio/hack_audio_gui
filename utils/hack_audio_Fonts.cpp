#include "hack_audio_FontData.cpp"

const juce::Typeface::Ptr ptr_NowLight       = juce::Typeface::createSystemTypefaceFor(NowLight_otf, NowLight_otfSize);
const juce::Typeface::Ptr ptr_NowRegular     = juce::Typeface::createSystemTypefaceFor(NowRegular_otf, NowRegular_otfSize);
const juce::Typeface::Ptr ptr_NowBold        = juce::Typeface::createSystemTypefaceFor(NowBold_otf, NowBold_otfSize);

const juce::Typeface::Ptr ptr_BaumansRegular = juce::Typeface::createSystemTypefaceFor(BaumansRegular_ttf, BaumansRegular_ttfSize);

const juce::Font HackAudio::Fonts::NowLight (ptr_NowLight);
const juce::Font HackAudio::Fonts::NowRegular     (ptr_NowRegular);
const juce::Font HackAudio::Fonts::NowBold (ptr_NowBold);
const juce::Font HackAudio::Fonts::Baumans (ptr_BaumansRegular);
