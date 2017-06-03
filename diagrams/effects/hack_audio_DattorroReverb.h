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

#ifndef HACK_AUDIO_DATTORROREVERB_H
#define HACK_AUDIO_DATTORROREVERB_H

struct DattorroReverb : public HackAudio::Diagram
{
    // Dattorro Block Diagram
    HackAudio::Label fbd_1_label;
    
    HackAudio::Label apf_1_label;
    HackAudio::Label apf_2_label;
    HackAudio::Label apf_3_label;
    HackAudio::Label apf_4_label;
    
    // Top or Left
    HackAudio::Label apf_5_label;
    HackAudio::Label delay_1_label;
    HackAudio::Label fbd_2_label;
    HackAudio::Label apf_6_label;
    HackAudio::Label delay_2_label;
    
    // Bottom or Right
    HackAudio::Label apf_7_label;
    HackAudio::Label delay_3_label;
    HackAudio::Label fbd_3_label;
    HackAudio::Label apf_8_label;
    HackAudio::Label delay_4_label;
    
    
    // Feedback Junctions
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction mul1;
    HackAudio::Diagram::Junction mul2;
    HackAudio::Diagram::Junction fb1;
    HackAudio::Diagram::Junction fb2;
    HackAudio::Diagram::Junction node1;
    HackAudio::Diagram::Junction node2;
    HackAudio::Diagram::Junction node3;
    HackAudio::Diagram::Junction node4;
    
    // Feedback Gains
    HackAudio::Label decay_1_label;
    HackAudio::Label decay_2_label;


    HackAudio::Diagrams::AllpassFilter apf_1_diag;
    HackAudio::Diagrams::AllpassFilter apf_2_diag;
    HackAudio::Diagrams::AllpassFilter apf_3_diag;
    HackAudio::Diagrams::AllpassFilter apf_4_diag;
    HackAudio::Diagrams::AllpassFilter apf_5_diag;
    HackAudio::Diagrams::AllpassFilter apf_6_diag;
    HackAudio::Diagrams::AllpassFilter apf_7_diag;
    HackAudio::Diagrams::AllpassFilter apf_8_diag;
    
    HackAudio::Diagrams::DampingFilter fbd_1_diag;
    HackAudio::Diagrams::DampingFilter fbd_2_diag;
    HackAudio::Diagrams::DampingFilter fbd_3_diag;


    DattorroReverb()
    {

        fbd_1_label.setPlaceholder("FBD");
        fbd_1_label.setBounds(12,0,64,40);
        
        apf_1_label.setPlaceholder("APF");
        apf_1_label.setBounds(97,0,64,40);
        apf_2_label.setPlaceholder("APF");
        apf_2_label.setBounds(182,0,64,40);
        apf_3_label.setPlaceholder("APF");
        apf_3_label.setBounds(267,0,64,40);
        apf_4_label.setPlaceholder("APF");
        apf_4_label.setBounds(352,0,64,40);

        // Sum Junctions
        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(375,-45,25,25);
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(375,60,25,25);
        
        // Top or Left
        apf_5_label.setPlaceholder("APF");
        apf_5_label.setBounds(350,-100,64,40);
        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(438,-100,64,40);
        fbd_2_label.setPlaceholder("FBD");
        fbd_2_label.setBounds(526,-100,64,40);
        apf_6_label.setPlaceholder("APF");
        apf_6_label.setBounds(614,-100,64,40);
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(702,-100,64,40);
        
        // Bottom or Right
        apf_7_label.setPlaceholder("APF");
        apf_7_label.setBounds(350,100,64,40);
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(438,100,64,40);
        fbd_3_label.setPlaceholder("FBD");
        fbd_3_label.setBounds(526,100,64,40);
        apf_8_label.setPlaceholder("APF");
        apf_8_label.setBounds(614,100,64,40);
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(702,100,64,40);
        
        // Feedback Junctions
        fb1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb1.setBounds(780,-85,10,10);
        fb2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb2.setBounds(780,115,10,10);
        
        mul1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul1.setBounds(652,-10,25,25);
        mul2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul2.setBounds(575,30,25,25);
        
        node1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        node1.setBounds(430,-10,0,0);
        node2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        node2.setBounds(430,30,0,0);
        node3.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        node3.setBounds(330,-65,0,0);
        node4.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        node4.setBounds(330,85,0,0);
        
        // Gains
        decay_1_label.setPlaceholder("Decay");
        decay_1_label.setBounds(590,-45,60,30);
        decay_2_label.setPlaceholder("Decay");
        decay_2_label.setBounds(500,28,60,30);
        
        // Connections
        addDiagramInput(fbd_1_label);
        connect(fbd_1_label,apf_1_label);
        connect(apf_1_label,apf_2_label);
        connect(apf_2_label,apf_3_label);
        connect(apf_3_label,apf_4_label);
        connect(apf_4_label,node1);
        connect(apf_4_label,node2);
        connect(node1,sum1);
        connect(node2,sum2);
        
        connect(sum1,node3,HackAudio::Diagram::Junction::Horizontal);
        connect(node3,apf_5_label);
        connect(apf_5_label,delay_1_label);
        connect(delay_1_label,fbd_2_label);
        connect(fbd_2_label,apf_6_label);
        connect(apf_6_label,delay_2_label);
        
        connect(sum2,node4,HackAudio::Diagram::Junction::Horizontal);
        connect(node4,apf_7_label);
        connect(apf_7_label,delay_3_label);
        connect(delay_3_label,fbd_3_label);
        connect(fbd_3_label,apf_8_label);
        connect(apf_8_label,delay_4_label);
        
        connect(fb1,mul1);
        // HackAudio::Diagram::Junction::Horizontal
        connect(mul1,sum2);
        connect(fb2,mul2);
        connect(mul2,sum1);
        
        // Gains
        connect(decay_1_label,mul1);
        connect(decay_2_label,mul2);
        
        // Outputs
        connect(delay_2_label,fb1);
        connect(delay_4_label,fb2);
        
        
        addDiagramOutput(fb1);
        addDiagramOutput(fb2);

        setSubDiagram(apf_1_label, apf_1_diag);
        setSubDiagram(apf_2_label, apf_2_diag);
        setSubDiagram(apf_3_label, apf_3_diag);
        setSubDiagram(apf_4_label, apf_4_diag);
        setSubDiagram(apf_5_label, apf_5_diag);
        setSubDiagram(apf_6_label, apf_6_diag);
        setSubDiagram(apf_7_label, apf_7_diag);
        setSubDiagram(apf_8_label, apf_8_diag);
        
        setSubDiagram(fbd_1_label, fbd_1_diag);
        setSubDiagram(fbd_2_label, fbd_2_diag);
        setSubDiagram(fbd_3_label, fbd_3_diag);

        setName("Dattorro Plate Reverb");

    }

};

#endif
