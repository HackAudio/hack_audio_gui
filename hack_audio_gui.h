/*

BEGIN_JUCE_MODULE_DECLARATION

	ID:			  	hack_audio_gui
	vendor:		  	HackAudio
	version:	  	0.1.0
	name:		  	HackAudio GUI
	description:	Custom JUCE UI for HackAudio Plugins
	website:		http://www.hackaudio.com
	license:		MIT

END_JUCE_MODULE_DECLARATION

*/

#ifndef HACK_AUDIO_GUI_INCLUDED
#define HACK_AUDIO_GUI_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

// User Interface Configurations

#define ANIMATION_SPEED 0.1
#define ANIMATION_FPS 60

#define CORNER_RADIUS   24, 24
#define CORNER_CONFIG   false, true, true, false

#define ROTARY_ANGLESTART ((7.0f * M_PI) / 6.0f)
#define ROTARY_ANGLERANGE ((5.0f * M_PI) / 3.0f)

#define DEFAULT_PIPMIN 6
#define DEFAULT_PIPMAX 8

#define DEFAULT_FONTSIZE 18

#define MOUSEWHEEL_SENSITIVITY 128

// ===================================================

#include "utils/hack_audio_Colours.h"
#include "utils/hack_audio_Fonts.h"

#include "components/hack_audio_Selector.h"
#include "components/hack_audio_Slider.h"
#include "components/hack_audio_Button.h"
#include "components/hack_audio_Label.h"
#include "components/hack_audio_Meter.h"

#include "layout/hack_audio_Diagram.h"
#include "layout/hack_audio_Viewport.h"
#include "layout/hack_audio_FlexBox.h"

#include "diagrams/hack_audio_Diagrams.h"

#endif
