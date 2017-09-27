/* Copyright (C) 2017 by Antonio Lassandro, HackAudio LLC
 *
 * hack_audio_gui is provided under the terms of The MIT License (MIT):
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

// =============================================================================

#include "utils/hack_audio_Colours.h"
#include "utils/hack_audio_Fonts.h"
#include "utils/hack_audio_NavigationButton.h"

#include "components/hack_audio_Selector.h"
#include "components/hack_audio_Slider.h"
#include "components/hack_audio_Button.h"
#include "components/hack_audio_Label.h"
#include "components/hack_audio_Meter.h"
#include "components/hack_audio_Graph.h"

#include "layout/hack_audio_Diagram.h"
#include "layout/hack_audio_Viewport.h"
#include "layout/hack_audio_FlexBox.h"

#include "diagrams/hack_audio_Diagrams.h"

#endif
