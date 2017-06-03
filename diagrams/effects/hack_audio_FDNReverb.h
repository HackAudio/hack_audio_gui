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

#ifndef HACK_AUDIO_FDNREVERB_H
#define HACK_AUDIO_FDNREVERB_H

struct FDNReverb : public HackAudio::Diagram
{
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction sum3;
    HackAudio::Diagram::Junction sum4;
    
    HackAudio::Diagram::Junction fbMult;
    
    HackAudio::Label delay_1_label;
    HackAudio::Label delay_2_label;
    HackAudio::Label delay_3_label;
    HackAudio::Label delay_4_label;
    
    HackAudio::Diagram::Junction fb1;
    HackAudio::Diagram::Junction fb2;
    HackAudio::Diagram::Junction fb3;
    HackAudio::Diagram::Junction fb4;
    
    HackAudio::Diagram::Junction sumOut;
    
    HackAudio::Label matrix_label;
    HackAudio::Label fbGain_label;
    
    HackAudio::Diagrams::Moddelay moddelay_1_diag;
    HackAudio::Diagrams::Moddelay moddelay_2_diag;
    HackAudio::Diagrams::Moddelay moddelay_3_diag;
    HackAudio::Diagrams::Moddelay moddelay_4_diag;
    ///

    FDNReverb()
    {
        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(90,-98,20,20);
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(60,-62,20,20);
        sum3.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum3.setBounds(30,-26,20,20);
        sum4.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum4.setBounds(0,10,20,20);
        
        sumOut.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sumOut.setBounds(500,-62,20,20);
        
        fbMult.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        fbMult.setBounds(170,85,20,20);

        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(310,-108,64,40);
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(250,-72,64,40);
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(190,-36,64,40);
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(130,0,64,40);
        
        matrix_label.setPlaceholder("Feed-back\nMatrix");
        matrix_label.setBounds(210,43,130,105);
        
        fbGain_label.setPlaceholder("Gain");
        fbGain_label.setBounds(100,108,50,32);
        
        fb1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb1.setBounds(450,-93,10,10);
        fb2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb2.setBounds(420,-57,10,10);
        fb3.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb3.setBounds(390,-21,10,10);
        fb4.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb4.setBounds(360,15,10,10);

        addDiagramInput(sum1);
        addDiagramInput(sum2);
        addDiagramInput(sum3);
        addDiagramInput(sum4);
        connect(sum1,delay_1_label);
        connect(sum2,delay_2_label);
        connect(sum3,delay_3_label);
        connect(sum4,delay_4_label);
        connect(delay_1_label,sumOut);
        connect(delay_2_label,sumOut);
        connect(delay_3_label,sumOut);
        connect(delay_4_label,sumOut);
        
        connect(fb1,matrix_label);
        connect(fb2,matrix_label);
        connect(fb3,matrix_label);
        connect(fb4,matrix_label);
        
        connect(matrix_label,fbMult);
        connect(fbGain_label,fbMult);
        
        connect(fbMult,sum1,HackAudio::Diagram::Junction::Horizontal);
        connect(fbMult,sum2,HackAudio::Diagram::Junction::Horizontal);
        connect(fbMult,sum3,HackAudio::Diagram::Junction::Horizontal);
        connect(fbMult,sum4,HackAudio::Diagram::Junction::Horizontal);
        
        addDiagramOutput(sumOut);

        setName("Feed-back Delay Network Reverb");
        
        setSubDiagram(delay_1_label, moddelay_1_diag);
        setSubDiagram(delay_2_label, moddelay_2_diag);
        setSubDiagram(delay_3_label, moddelay_3_diag);
        setSubDiagram(delay_4_label, moddelay_4_diag);

    }

};

#endif
