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

#ifndef HACK_AUDIO_DIAGRAMS_H
#define HACK_AUDIO_DIAGRAMS_H

namespace HackAudio
{

namespace Diagrams
{

    #include "utils/hack_audio_Gain.h"
    #include "utils/hack_audio_preDelay.h"
    #include "utils/hack_audio_Moddelay.h"

    #include "filters/hack_audio_AllpassFilter.h"
    #include "filters/hack_audio_CombFilter.h"
    #include "filters/hack_audio_DampingFilter.h"
    #include "filters/hack_audio_LPCombFilter.h"
    #include "filters/hack_audio_BiquadFilter.h"
    #include "filters/hack_audio_NestedallpassFilter.h"
    #include "filters/hack_audio_NestedtwoFilter.h"

    #include "effects/hack_audio_SchroederReverb.h"
    #include "effects/hack_audio_EarlyReflections.h"
    #include "effects/hack_audio_MoorerReverb.h"
    #include "effects/hack_audio_FDNReverb.h"
    #include "effects/hack_audio_DattorroReverb.h"
    #include "effects/hack_audio_GardnersmallReverb.h"
    #include "effects/hack_audio_GardnermediumReverb.h"
    #include "effects/hack_audio_GardnerlargeReverb.h"

};

}

#endif
