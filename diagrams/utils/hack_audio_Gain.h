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

#ifndef HACK_AUDIO_GAIN_H
#define HACK_AUDIO_GAIN_H

struct Gain : public HackAudio::Diagram
{

    HackAudio::Label main_db;
    HackAudio::Label main_linear;
    HackAudio::Diagram::Junction main_mult;

    Gain()
    {

        setName("Gain");

        main_db.setPlaceholder("Decibel");
        main_db.setBounds(0, -64, 96, 64);

        main_linear.setPlaceholder("10^{(dB/20)}");
        main_linear.setBounds(256, -64, 128, 64);

        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(512, 64, 40, 40);

        addDiagramInput(main_mult);

        connect(main_db, main_linear);
        connect(main_linear, main_mult);

        addDiagramOutput(main_mult);

    }

};

#endif

