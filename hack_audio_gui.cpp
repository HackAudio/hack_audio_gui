#ifdef HACK_AUDIO_UI_INCLUDED
    #error "Incorrect use of JUCE cpp file"
#endif

#include "hack_audio_gui.h"

const juce::Colour HackAudio::Colours::Black (0xFF010D14);
const juce::Colour HackAudio::Colours::Gray  (0xFF343D3E);
const juce::Colour HackAudio::Colours::White (0xFFCAD8DE);
const juce::Colour HackAudio::Colours::Cyan  (0xFF4DDDDB);

#include "components/hack_audio_Selector.cpp"
#include "components/hack_audio_Slider.cpp"
#include "components/hack_audio_Button.cpp"
#include "components/hack_audio_Label.cpp"
#include "components/hack_audio_Meter.cpp"

#include "layout/hack_audio_Diagram.cpp"
#include "layout/hack_audio_Viewport.cpp"
#include "layout/hack_audio_FlexBox.cpp"
