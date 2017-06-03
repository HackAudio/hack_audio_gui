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

#ifndef HACK_AUDIO_PREDELAY_H
#define HACK_AUDIO_PREDELAY_H

struct Predelay : public HackAudio::Diagram
{

    HackAudio::Label main_ms;
    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_node;

    Predelay()
    {

        setName("Pre-delay");

        main_ms.setPlaceholder("Milliseconds");
        main_ms.setBounds(0, -80, 96, 64);

        main_delay.setPlaceholder("Samples of\nDelay");
        main_delay.setBounds(150, 0, 128, 64);
        
        main_node.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        main_node.setBounds(214,0,0,0);

        addDiagramInput(main_delay);

        connect(main_ms, main_node);

        addDiagramOutput(main_delay);

    }

};

#endif

