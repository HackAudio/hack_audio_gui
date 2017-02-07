#ifdef HACK_AUDIO_UI_INCLUDED
    #error "Incorrect use of JUCE cpp file"
#endif

#include "hack_audio.h"

namespace HackAudio
{

    const juce::Colour Colours::Black (0xFF010D13);
	const juce::Colour Colours::Gray  (0xFF1E2324);
	const juce::Colour Colours::White (0xFFCAD8DD);
	const juce::Colour Colours::Cyan  (0xFF59C6C3);

    #include "components/hack_audio_Slider.cpp"
    #include "components/hack_audio_Button.cpp"

}

