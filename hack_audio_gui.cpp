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

#ifdef HACK_AUDIO_UI_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
  already included any other headers - just put it inside a file on its own, possibly with your config
  flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
  header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

/* HackAudio GUI Configurations */
// =============================================================================

#define ANIMATION_SPEED 0.1
#define ANIMATION_FPS 60

#define CORNER_RADIUS   24
#define CORNER_CONFIG   CORNER_RADIUS, CORNER_RADIUS, false, true, true, false

#define ROTARY_ANGLESTART ((7.0f * juce::float_Pi) / 6.0f)
#define ROTARY_ANGLERANGE ((5.0f * juce::float_Pi) / 3.0f)

#define DEFAULT_PIPMIN 6
#define DEFAULT_PIPMAX 8

#define MOUSEWHEEL_SENSITIVITY 128

// =============================================================================

#include "hack_audio_gui.h"

#include "utils/hack_audio_Colours.cpp"
#include "utils/hack_audio_Fonts.cpp"

#include "components/hack_audio_Selector.cpp"
#include "components/hack_audio_Slider.cpp"
#include "components/hack_audio_Button.cpp"
#include "components/hack_audio_Label.cpp"
#include "components/hack_audio_Meter.cpp"
#include "components/hack_audio_Graph.cpp"

#include "layout/hack_audio_Diagram.cpp"
#include "layout/hack_audio_Viewport.cpp"
#include "layout/hack_audio_FlexBox.cpp"
