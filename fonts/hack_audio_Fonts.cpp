#include "BinaryData.cpp"

const juce::Typeface::Ptr fira = juce::Typeface::createSystemTypefaceFor(BinaryData::FiraSansRegular_otf, BinaryData::FiraSansRegular_otfSize);

const juce::Font HackAudio::Fonts::Fira(fira);

const juce::Typeface::Ptr baumans = juce::Typeface::createSystemTypefaceFor(BinaryData::BaumansRegular_ttf, BinaryData::BaumansRegular_ttfSize);

const juce::Font HackAudio::Fonts::Baumans(baumans);
