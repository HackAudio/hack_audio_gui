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

#ifndef HACK_AUDIO_BIQUADFILTER_H
#define HACK_AUDIO_BIQUADFILTER_H

struct BiquadFilter : public HackAudio::Diagram
{

    HackAudio::Label main_dry;
    HackAudio::Diagram::Junction main_sum;
    HackAudio::Diagram::Junction main_mult;

    HackAudio::Label ff_1_delay;
    HackAudio::Label ff_1_gain;
    HackAudio::Diagram::Junction ff_1_mult;

    HackAudio::Label ff_2_delay;
    HackAudio::Label ff_2_gain;
    HackAudio::Diagram::Junction ff_2_mult;

    HackAudio::Label fb_1_delay;
    HackAudio::Label fb_1_gain;
    HackAudio::Diagram::Junction fb_1_mult;

    HackAudio::Label fb_2_delay;
    HackAudio::Label fb_2_gain;
    HackAudio::Diagram::Junction fb_2_mult;

    HackAudio::Diagram::Junction fb_node;

    BiquadFilter()
    {

        main_dry.setPlaceholder("b_0");
        main_dry.setBounds(85, -48, 55, 32);
        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(176, 5, 30, 30);
        main_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum.setBounds(250, 5, 30, 30);

        ff_1_delay.setText("z^{-1}", juce::dontSendNotification);
        ff_1_delay.setBounds(0, -115, 64, 40);
        ff_1_gain.setPlaceholder("b_1");
        ff_1_gain.setBounds(85, -150, 55, 32);
        ff_1_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_1_mult.setBounds(176, -110, 30, 30);

        ff_2_delay.setText("z^{-2}", juce::dontSendNotification);
        ff_2_delay.setBounds(0, -200, 64, 40);
        ff_2_gain.setPlaceholder("b_2");
        ff_2_gain.setBounds(85, -230, 55, 32);
        ff_2_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_2_mult.setBounds(176, -195, 30, 30);

        fb_1_delay.setText("z^{-1}", juce::dontSendNotification);
        fb_1_delay.setBounds(512, -115, 64, 40);
        fb_1_gain.setPlaceholder("-a_1");
        fb_1_gain.setBounds(410, -150, 55, 32);
        fb_1_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        fb_1_mult.setBounds(336, -110, 30, 30);

        fb_2_delay.setText("z^{-2}", juce::dontSendNotification);
        fb_2_delay.setBounds(512, -200, 64, 40);
        fb_2_gain.setPlaceholder("-a_2");
        fb_2_gain.setBounds(410, -230, 55, 32);
        fb_2_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        fb_2_mult.setBounds(336, -195, 30, 30);

        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(625, 12, 15, 15);

        addDiagramInput(main_mult);
        addDiagramInput(ff_1_delay);
        addDiagramInput(ff_2_delay);

        connect(main_dry, main_mult);
        connect(main_mult, main_sum);

        connect(ff_1_delay, ff_1_mult);
        connect(ff_1_gain, ff_1_mult);
        connect(ff_1_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);

        connect(ff_2_delay, ff_2_mult);
        connect(ff_2_gain, ff_2_mult);
        connect(ff_2_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);

        connect(fb_1_delay, fb_1_mult);
        connect(fb_1_gain, fb_1_mult);
        connect(fb_1_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);

        connect(fb_2_delay, fb_2_mult);
        connect(fb_2_gain, fb_2_mult);
        connect(fb_2_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);

        connect(fb_node, fb_1_delay);
        connect(fb_node, fb_2_delay);

        addDiagramOutput(main_sum);

        setName("Bi-Quad Low-Pass Filter");

    }

};

#endif
