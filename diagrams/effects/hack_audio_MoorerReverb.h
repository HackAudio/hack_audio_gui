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

#ifndef HACK_AUDIO_MOORERREVERB_H
#define HACK_AUDIO_MOORERREVERB_H

struct MoorerReverb : public HackAudio::Diagram
{
    HackAudio::Label earlyRef_label;
    
    HackAudio::Label comb_2_label;
    HackAudio::Label comb_3_label;
    HackAudio::Label comb_4_label;
    HackAudio::Label comb_5_label;
    HackAudio::Label comb_6_label;
    HackAudio::Label comb_7_label;

    HackAudio::Diagram::Junction comb_sum;

    HackAudio::Label apf_1_label;
    HackAudio::Label apf_2_label;
    HackAudio::Label apf_3_label;

    HackAudio::Diagrams::EarlyReflections earlyRef_diag;
    
    HackAudio::Diagrams::LPCombFilter comb_2_diag;
    HackAudio::Diagrams::LPCombFilter comb_3_diag;
    HackAudio::Diagrams::LPCombFilter comb_4_diag;
    HackAudio::Diagrams::LPCombFilter comb_5_diag;
    HackAudio::Diagrams::LPCombFilter comb_6_diag;
    HackAudio::Diagrams::LPCombFilter comb_7_diag;


    HackAudio::Diagrams::AllpassFilter apf_1_diag;
    HackAudio::Diagrams::AllpassFilter apf_2_diag;
    HackAudio::Diagrams::AllpassFilter apf_3_diag;

    MoorerReverb()
    {
        earlyRef_label.setPlaceholder("ER");
        earlyRef_label.setBounds(0,-8,72,64);

        comb_2_label.setPlaceholder("LPCF");
        comb_2_label.setBounds(240, -90, 64, 48);
        comb_3_label.setPlaceholder("LPCF");
        comb_3_label.setBounds(155, -55, 64, 48);
        comb_4_label.setPlaceholder("LPCF");
        comb_4_label.setBounds(240, -20, 64, 48);
        comb_5_label.setPlaceholder("LPCF");
        comb_5_label.setBounds(155, 20, 64, 48);
        comb_6_label.setPlaceholder("LPCF");
        comb_6_label.setBounds(240, 55, 64, 48);
        comb_7_label.setPlaceholder("LPCF");
        comb_7_label.setBounds(155, 90, 64, 48);


        comb_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        comb_sum.setBounds(350, 10, 30, 30);

        apf_1_label.setPlaceholder("APF");
        apf_1_label.setBounds(425, 0, 64, 48);
        apf_2_label.setPlaceholder("APF");
        apf_2_label.setBounds(525, 0, 64, 48);
        apf_3_label.setPlaceholder("APF");
        apf_3_label.setBounds(625, 0, 64, 48);

        addDiagramInput(earlyRef_label);
        connect(earlyRef_label,comb_2_label);
        connect(earlyRef_label,comb_3_label);
        connect(earlyRef_label,comb_4_label);
        connect(earlyRef_label,comb_5_label);
        connect(earlyRef_label,comb_6_label);
        connect(earlyRef_label,comb_7_label);

        connect(comb_2_label, comb_sum);
        connect(comb_3_label, comb_sum);
        connect(comb_4_label, comb_sum);
        connect(comb_5_label, comb_sum);
        connect(comb_6_label, comb_sum);
        connect(comb_7_label, comb_sum);

        connect(comb_sum, apf_1_label);
        connect(apf_1_label, apf_2_label);
        connect(apf_2_label, apf_3_label);

        addDiagramOutput(apf_3_label);

        setSubDiagram(comb_2_label, comb_2_diag);
        setSubDiagram(comb_3_label, comb_3_diag);
        setSubDiagram(comb_4_label, comb_4_diag);
        setSubDiagram(comb_5_label, comb_5_diag);
        setSubDiagram(comb_6_label, comb_6_diag);
        setSubDiagram(comb_7_label, comb_7_diag);

        setSubDiagram(apf_1_label, apf_1_diag);
        setSubDiagram(apf_2_label, apf_2_diag);
        setSubDiagram(apf_3_label, apf_3_diag);

        setSubDiagram(earlyRef_label, earlyRef_diag);
        
        setName("Moorer Reverb");

    }

};

#endif
