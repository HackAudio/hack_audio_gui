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

#ifndef HACK_AUDIO_GARDNERLARGEREVERB_H
#define HACK_AUDIO_GARDNERLARGEREVERB_H

struct GardnerlargeReverb : public HackAudio::Diagram
{

    
    HackAudio::Label apf_1_label;
    HackAudio::Label apf_2_label;
    HackAudio::Label napf_1_label;
    HackAudio::Label napf_2_label;
    
    HackAudio::Label lpf_label;
    
    HackAudio::Label delay_1_label;
    HackAudio::Label delay_2_label;
    HackAudio::Label delay_3_label;
    HackAudio::Label delay_4_label;
    
    HackAudio::Label gain_0_label;
    HackAudio::Label gain_1_label;
    HackAudio::Label gain_2_label;
    HackAudio::Label gain_3_label;
    
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction sum3;
    HackAudio::Diagram::Junction mul0;
    HackAudio::Diagram::Junction mul1;
    HackAudio::Diagram::Junction mul2;
    HackAudio::Diagram::Junction mul3;
    HackAudio::Diagram::Junction node0;
    HackAudio::Diagram::Junction node1;
    HackAudio::Diagram::Junction node2;

    HackAudio::Diagrams::BiquadFilter lpf_diag;
    HackAudio::Diagrams::AllpassFilter apf_1_diag;
    HackAudio::Diagrams::AllpassFilter apf_2_diag;
    HackAudio::Diagrams::NestedallpassFilter napf_1_diag;
    HackAudio::Diagrams::NestedtwoFilter napf_2_diag;

    GardnerlargeReverb()
    {

        //////// Gardner LARGE/////////////////////////////////////////
        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(0,8,30,30);
        
        apf_1_label.setPlaceholder("APF");
        apf_1_label.setBounds(45,0,60,48);
        
        apf_2_label.setPlaceholder("APF");
        apf_2_label.setBounds(130,0,60,48);
        
        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(215,0,60,48);
        
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(310,0,60,48);
        
        napf_1_label.setPlaceholder("NAPF");
        napf_1_label.setBounds(392,0,60,48);
        
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(474,0,60,48);
        
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(566,0,60,48);
        
        napf_2_label.setPlaceholder("2NAPF");
        napf_2_label.setBounds(647,0,60,48);
        
        lpf_label.setPlaceholder("LPF");
        lpf_label.setBounds(256,95,60,48);
        
        gain_1_label.setPlaceholder("Gain");
        gain_1_label.setBounds(460,-48,50,30);
        
        gain_2_label.setPlaceholder("Gain");
        gain_2_label.setBounds(650,-48,50,30);
        
        gain_3_label.setPlaceholder("Gain");
        gain_3_label.setBounds(100,64,50,30);
        
        // Output
        node1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node1.setBounds(545,18,12,12);
        
        node2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node2.setBounds(735,64,0,0);
        
        mul1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul1.setBounds(535,-48,30,30);
        
        mul2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul2.setBounds(720,-48,30,30);
        
        mul3.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul3.setBounds(150,104,30,30);
        
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(721,-96,30,30);
        
        sum3.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum3.setBounds(580,-96,30,30);
        
        gain_0_label.setPlaceholder("Gain");
        gain_0_label.setBounds(205,-96,50,30);
        mul0.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul0.setBounds(280,-96,30,30);
        node0.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node0.setBounds(290,18,12,12);
        

        // Connections
        addDiagramInput(sum1);
        connect(sum1,apf_1_label);
        connect(apf_1_label,apf_2_label);
        connect(apf_2_label,delay_1_label);
        connect(delay_1_label,delay_2_label);
        connect(delay_2_label,napf_1_label);
        connect(napf_1_label,delay_3_label);
        connect(delay_3_label,delay_4_label);
        connect(delay_4_label,napf_2_label);
        connect(napf_2_label,mul2);
        
        connect(napf_2_label,node2);
        connect(node2,lpf_label);
        connect(lpf_label,mul3);
        
        connect(gain_3_label,mul3);
        connect(mul3,sum1,HackAudio::Diagram::Junction::Horizontal);
        
        connect(node1,mul1);
        connect(gain_1_label,mul1);
        connect(mul1,sum3,HackAudio::Diagram::Junction::Horizontal);
        connect(gain_2_label,mul2);
        connect(mul2,sum2,HackAudio::Diagram::Junction::Horizontal);
        connect(sum3,sum2);
        
        connect(node0,mul0);
        connect(gain_0_label,mul0);
        connect(mul0,sum3);
        addDiagramOutput(sum2);
        
        
        setName("Gardner Large Reverb");
        
        setSubDiagram(lpf_label,lpf_diag);
        setSubDiagram(apf_1_label,apf_1_diag);
        setSubDiagram(apf_2_label,apf_2_diag);
        setSubDiagram(napf_1_label,napf_1_diag);
        setSubDiagram(napf_2_label,napf_2_diag);
    }

};

#endif
