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

#ifndef HACK_AUDIO_EARLYREFLECTIONS_H
#define HACK_AUDIO_EARLYREFLECTIONS_H

struct EarlyReflections : public HackAudio::Diagram
{
    
    HackAudio::Label delay_1_label;
    HackAudio::Label delay_2_label;
    HackAudio::Label delay_3_label;
    HackAudio::Label delay_4_label;
    HackAudio::Label delay_5_label;
    HackAudio::Label delay_6_label;
    HackAudio::Label delay_7_label;
    HackAudio::Label delay_8_label;
    
    HackAudio::Label gain_1_label;
    HackAudio::Label gain_2_label;
    HackAudio::Label gain_3_label;
    HackAudio::Label gain_4_label;
    HackAudio::Label gain_5_label;
    HackAudio::Label gain_6_label;
    HackAudio::Label gain_7_label;
    HackAudio::Label gain_8_label;
    
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction sum3;
    HackAudio::Diagram::Junction sum4;
    HackAudio::Diagram::Junction sum5;
    HackAudio::Diagram::Junction sum6;
    HackAudio::Diagram::Junction sum7;
    HackAudio::Diagram::Junction sum8;
    
    HackAudio::Diagram::Junction mul1;
    HackAudio::Diagram::Junction mul2;
    HackAudio::Diagram::Junction mul3;
    HackAudio::Diagram::Junction mul4;
    HackAudio::Diagram::Junction mul5;
    HackAudio::Diagram::Junction mul6;
    HackAudio::Diagram::Junction mul7;
    HackAudio::Diagram::Junction mul8;

    EarlyReflections()
    {
        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(0,-100,64,50);
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(96,-100,64,50);
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(192,-100,64,50);
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(288,-100,64,50);
        delay_5_label.setPlaceholder("Delay");
        delay_5_label.setBounds(384,-100,64,50);
        delay_6_label.setPlaceholder("Delay");
        delay_6_label.setBounds(480,-100,64,50);
        delay_7_label.setPlaceholder("Delay");
        delay_7_label.setBounds(576,-100,64,50);
        delay_8_label.setPlaceholder("Delay");
        delay_8_label.setBounds(672,-100,64,50);


        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(70,100,30,30);
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(166,100,30,30);
        sum3.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum3.setBounds(262,100,30,30);
        sum4.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum4.setBounds(358,100,30,30);
        sum5.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum5.setBounds(454,100,30,30);
        sum6.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum6.setBounds(550,100,30,30);
        sum7.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum7.setBounds(646,100,30,30);
        sum8.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum8.setBounds(750,100,30,30);
        
        mul1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul1.setBounds(70,5,30,30);
        mul2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul2.setBounds(166,5,30,30);
        mul3.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul3.setBounds(262,5,30,30);
        mul4.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul4.setBounds(358,5,30,30);
        mul5.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul5.setBounds(454,5,30,30);
        mul6.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul6.setBounds(550,5,30,30);
        mul7.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul7.setBounds(646,5,30,30);
        mul8.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul8.setBounds(750,5,30,30);
        
        gain_1_label.setPlaceholder("g1");
        gain_1_label.setBounds(12,5,40,30);
        gain_2_label.setPlaceholder("g2");
        gain_2_label.setBounds(108,5,40,30);
        gain_3_label.setPlaceholder("g3");
        gain_3_label.setBounds(204,5,40,30);
        gain_4_label.setPlaceholder("g4");
        gain_4_label.setBounds(300,5,40,30);
        gain_5_label.setPlaceholder("g5");
        gain_5_label.setBounds(396,5,40,30);
        gain_6_label.setPlaceholder("g6");
        gain_6_label.setBounds(492,5,40,30);
        gain_7_label.setPlaceholder("g7");
        gain_7_label.setBounds(588,5,40,30);
        gain_8_label.setPlaceholder("g8");
        gain_8_label.setBounds(684,5,40,30);

        addDiagramInput(delay_1_label);
        connect(delay_1_label,delay_2_label);
        connect(delay_2_label,delay_3_label);
        connect(delay_3_label,delay_4_label);
        connect(delay_4_label,delay_5_label);
        connect(delay_5_label,delay_6_label);
        connect(delay_6_label,delay_7_label);
        connect(delay_7_label,delay_8_label);
        connect(delay_8_label,mul8);
        
        
        addDiagramInput(sum1);
        connect(sum1,sum2);
        connect(sum2,sum3);
        connect(sum3,sum4);
        connect(sum4,sum5);
        connect(sum5,sum6);
        connect(sum6,sum7);
        connect(sum7,sum8);
        addDiagramOutput(sum8);
        
        connect(delay_1_label,mul1);
        connect(delay_2_label,mul2);
        connect(delay_3_label,mul3);
        connect(delay_4_label,mul4);
        connect(delay_5_label,mul5);
        connect(delay_6_label,mul6);
        connect(delay_7_label,mul7);
        
        connect(gain_1_label,mul1);
        connect(gain_2_label,mul2);
        connect(gain_3_label,mul3);
        connect(gain_4_label,mul4);
        connect(gain_5_label,mul5);
        connect(gain_6_label,mul6);
        connect(gain_7_label,mul7);
        connect(gain_8_label,mul8);
        
        connect(mul1,sum1);
        connect(mul2,sum2);
        connect(mul3,sum3);
        connect(mul4,sum4);
        connect(mul5,sum5);
        connect(mul6,sum6);
        connect(mul7,sum7);
        connect(mul8,sum8);
        
        setName("Early-Reflections Network");

    }

};

#endif
