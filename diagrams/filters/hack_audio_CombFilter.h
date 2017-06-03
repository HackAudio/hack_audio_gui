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

#ifndef HACK_AUDIO_COMBFILTER_H
#define HACK_AUDIO_COMBFILTER_H

struct CombFilter : public HackAudio::Diagram
{

    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_sum;
    HackAudio::Diagram::Junction main_mult;

    HackAudio::Label fb_gain;
    HackAudio::Diagram::Junction fb_node;
    
    HackAudio::Diagrams::Moddelay moddelay_diag;

    CombFilter()
    {

        main_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum.setBounds(250, 10, 30, 30);
        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(300, -64, 30, 30);
        main_delay.setPlaceholder("Delay");
        main_delay.setBounds(400, -7.5, 96, 64);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(350, -128, 64, 48);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(550, 17, 15, 15);

        addDiagramInput(main_sum);

        connect(main_sum, main_delay);
        connect(fb_node, main_mult);
        connect(fb_gain, main_mult);
        connect(main_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);
        connect(main_delay, fb_node);

        addDiagramOutput(fb_node);

        setName("Feed-Back Comb Filter");
        
        setSubDiagram(main_delay, moddelay_diag);

    }

};

#endif
