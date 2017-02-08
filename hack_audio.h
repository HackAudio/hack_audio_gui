/*

BEGIN_JUCE_MODULE_DECLARATION

	ID:			  	hack_audio
	vendor:		  	HackAudio
	version:	  	0.1.0
	name:		  	HackAudio
	description:	Custom JUCE UI for HackAudio Plugins
	website:		http://www.hackaudio.com
	license:		MIT

END_JUCE_MODULE_DECLARATION

*/

#ifndef HACK_AUDIO_UI_INCLUDED
#define HACK_AUDIO_UI_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

#define ANIMATION_SPEED 0.1
#define CORNER_RADIUS   24

namespace HackAudio
{

    struct Colours
    {

        static const juce::Colour
        Black,
        Gray,
        White,
        Cyan;

    };

    class Slider;
    class Button;

    #include "components/hack_audio_Slider.h"
    #include "components/hack_audio_Button.h"

}

#endif
