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

#ifndef HACK_AUDIO_GARDNERSMALLREVERB_H
#define HACK_AUDIO_GARDNERSMALLREVERB_H

struct GardnersmallReverb : public HackAudio::Diagram
{

    HackAudio::Label main_delay_label;
    HackAudio::Label napf_1_label;
    HackAudio::Label napf_2_label;
    HackAudio::Label lpf_label;
    
    HackAudio::Label gain_1_label;
    HackAudio::Label gain_2_label;
    HackAudio::Label gain_3_label;
    
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction mul1;
    HackAudio::Diagram::Junction mul2;
    HackAudio::Diagram::Junction mul3;
    HackAudio::Diagram::Junction node1;
    HackAudio::Diagram::Junction node2;

    HackAudio::Diagrams::BiquadFilter lpf_diag;
    HackAudio::Diagrams::NestedtwoFilter napf_1_diag;
    HackAudio::Diagrams::NestedallpassFilter napf_2_diag;


    GardnersmallReverb()
    {

        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(0,8,30,30);
        
        main_delay_label.setPlaceholder("Delay");
        //main_delay_label.setText("24",dontSendNotification);
        main_delay_label.setBounds(70,0,64,48);
        
        
        napf_1_label.setPlaceholder("2NAPF");
        //napf_1_label.setText("35",dontSendNotification);
        napf_1_label.setBounds(192,0,64,48);
        
        napf_2_label.setPlaceholder("NAPF");
        //napf_2_label.setText("66",dontSendNotification);
        napf_2_label.setBounds(320,0,64,48);
        
        lpf_label.setPlaceholder("LPF");
        //lpf_label.setText("4200",dontSendNotification);
        lpf_label.setBounds(256,95,64,48);
        
        gain_1_label.setPlaceholder("Gain");
        //gain_1_label.setText("0.5",dontSendNotification);
        gain_1_label.setBounds(200,-96,50,30);
        
        gain_2_label.setPlaceholder("Gain");
        //gain_2_label.setText("0.5",dontSendNotification);
        gain_2_label.setBounds(365,-48,50,30);
        
        gain_3_label.setPlaceholder("Gain");
        gain_3_label.setBounds(100,64,50,30);
        
        
        node1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node1.setBounds(285,18,12,12); // BUG? 284
        
        node2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node2.setBounds(410,64,0,0);
        
        mul1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul1.setBounds(275,-96,30,30);
        
        mul2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul2.setBounds(440,-48,30,30);
        
        mul3.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul3.setBounds(150,104,30,30);
        
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(500,-96,30,30);
        

        addDiagramInput(sum1);
        connect(sum1,main_delay_label);
        connect(main_delay_label,napf_1_label);
        connect(napf_1_label,napf_2_label);
        connect(napf_2_label,mul2);
        connect(napf_2_label,node2);
        connect(node2,lpf_label);
        connect(lpf_label,mul3);
        
        connect(gain_3_label,mul3);
        connect(mul3,sum1,HackAudio::Diagram::Junction::Horizontal);
        
        connect(node1,mul1);
        connect(gain_1_label,mul1);
        connect(mul1,sum2);
        connect(gain_2_label,mul2);
        connect(mul2,sum2,HackAudio::Diagram::Junction::Horizontal);
        addDiagramOutput(sum2);
        
        
        setName("Gardner Small Reverb");
        
        
        setSubDiagram(lpf_label, lpf_diag);
        setSubDiagram(napf_1_label, napf_1_diag);
        setSubDiagram(napf_2_label, napf_2_diag);
        
    }

};

#endif
