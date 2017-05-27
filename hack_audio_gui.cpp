#ifdef HACK_AUDIO_UI_INCLUDED
    #error "Incorrect use of JUCE cpp file"
#endif

#include "hack_audio_gui.h"

const juce::Colour HackAudio::Colours::Black (0xFF10151D);
const juce::Colour HackAudio::Colours::Gray  (0xFF3E4C5C);
const juce::Colour HackAudio::Colours::White (0xFFFFFFFF);
const juce::Colour HackAudio::Colours::Cyan  (0xFF11F2DE);

#include "fonts/hack_audio_Fonts.cpp"

#include "components/hack_audio_Selector.cpp"
#include "components/hack_audio_Slider.cpp"
#include "components/hack_audio_Button.cpp"
#include "components/hack_audio_Label.cpp"
#include "components/hack_audio_Meter.cpp"

#include "layout/hack_audio_Diagram.cpp"
#include "layout/hack_audio_Viewport.cpp"
#include "layout/hack_audio_FlexBox.cpp"
