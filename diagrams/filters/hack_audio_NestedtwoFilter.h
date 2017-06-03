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

#ifndef HACK_AUDIO_NESTEDTWOFILTER_H
#define HACK_AUDIO_NESTEDTWOFILTER_H

struct NestedtwoFilter : public HackAudio::Diagram
{

    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_sum_1;
    HackAudio::Diagram::Junction main_sum_2;

    HackAudio::Label fb_gain;
    HackAudio::Diagram::Junction fb_mult;
    HackAudio::Diagram::Junction fb_node;

    HackAudio::Label ff_gain;
    HackAudio::Diagram::Junction ff_mult;
    HackAudio::Diagram::Junction ff_node;
    
    HackAudio::Label apf_1_label;
    HackAudio::Label apf_2_label;
    
    HackAudio::Diagrams::AllpassFilter apf_1_diag;
    HackAudio::Diagrams::AllpassFilter apf_2_diag;

    NestedtwoFilter()
    {
        apf_1_label.setPlaceholder("APF");
        apf_1_label.setBounds(410, -4, 88, 56);
        apf_2_label.setPlaceholder("APF");
        apf_2_label.setBounds(520, -4, 88, 56);
        
        main_delay.setPlaceholder("Delay");
        main_delay.setBounds(300, -4, 88, 56);
        main_sum_1.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_1.setBounds(200, 9, 30, 30);
        main_sum_2.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_2.setBounds(650, 9, 30, 30);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(130, -100, 64, 48);
        fb_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        fb_mult.setBounds(275, -50, 30, 30);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(630, 15, 15, 15);

        ff_gain.setPlaceholder("Gain");
        ff_gain.setBounds(640, 100, 64, 48);
        ff_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_mult.setBounds(550, 70, 30, 30);
        ff_node.setSymbol(HackAudio::Diagram::Junction::None);
        ff_node.setBounds(250, 15, 15, 15);

        addDiagramInput(main_sum_1);

        connect(main_sum_1, main_delay);
        connect(main_delay, apf_1_label);
        connect(apf_1_label, apf_2_label);
        connect(apf_2_label, main_sum_2);

        connect(fb_node, fb_mult);
        connect(fb_gain, fb_mult);
        connect(fb_mult, main_sum_1,HackAudio::Diagram::Junction::Horizontal);

        connect(ff_node, ff_mult);
        connect(ff_gain, ff_mult);
        connect(ff_mult, main_sum_2, HackAudio::Diagram::Junction::Horizontal);

        addDiagramOutput(main_sum_2);

        setName("Double-Nested, All-Pass Filter");
        
        setSubDiagram(apf_1_label, apf_1_diag);
        setSubDiagram(apf_2_label, apf_2_diag);

    }

};

#endif
