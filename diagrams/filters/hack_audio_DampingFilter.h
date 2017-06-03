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

#ifndef HACK_AUDIO_DAMPINGFILTER_H
#define HACK_AUDIO_DAMPINGFILTER_H

struct DampingFilter : public HackAudio::Diagram
{

    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_sum;
    HackAudio::Diagram::Junction main_mult;
    
    HackAudio::Label fb_gain;
    HackAudio::Diagram::Junction fb_node;
    
    HackAudio::Diagram::Junction ff_mult;
    HackAudio::Label ff_gain;
    
    
    DampingFilter()
    {

        main_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum.setBounds(250, 10, 30, 30);
        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(300, -64, 30, 30);
        main_delay.setPlaceholder("z^{-1}");
        main_delay.setBounds(400, -81, 96, 64);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(200, -128, 64, 48);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(550, 17, 15, 15);
        
        ff_gain.setPlaceholder("Gain");
        ff_gain.setBounds(100, -64, 64, 48);
        ff_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_mult.setBounds(180, 10, 30, 30);

        addDiagramInput(ff_mult);
        connect(ff_gain,ff_mult);
        connect(ff_mult,main_sum);
        //connect(main_sum, main_delay);
        connect(fb_node, main_delay);
        connect(main_delay, main_mult);
        connect(fb_gain, main_mult);
        connect(main_mult, main_sum,HackAudio::Diagram::Junction::Horizontal);

        addDiagramOutput(main_sum);

        setName("Feed-Back Comb Filter");
        
        

    }

};

#endif
