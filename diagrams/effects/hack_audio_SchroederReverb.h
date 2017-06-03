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

#ifndef HACK_AUDIO_SCHROEDERREVERB_H
#define HACK_AUDIO_SCHROEDERREVERB_H

struct SchroederReverb : public HackAudio::Diagram
{

    HackAudio::Label comb_1_label;
    HackAudio::Label comb_2_label;
    HackAudio::Label comb_3_label;
    HackAudio::Label comb_4_label;
    HackAudio::Label comb_5_label;
    HackAudio::Label comb_6_label;
    HackAudio::Label comb_7_label;
    HackAudio::Label comb_8_label;

    HackAudio::Diagram::Junction comb_sum;

    HackAudio::Label apf_1_label;
    HackAudio::Label apf_2_label;
    HackAudio::Label apf_3_label;

    HackAudio::Diagrams::CombFilter comb_1_diag;
    HackAudio::Diagrams::CombFilter comb_2_diag;
    HackAudio::Diagrams::CombFilter comb_3_diag;
    HackAudio::Diagrams::CombFilter comb_4_diag;
    HackAudio::Diagrams::CombFilter comb_5_diag;
    HackAudio::Diagrams::CombFilter comb_6_diag;
    HackAudio::Diagrams::CombFilter comb_7_diag;
    HackAudio::Diagrams::CombFilter comb_8_diag;

    HackAudio::Diagrams::AllpassFilter apf_1_diag;
    HackAudio::Diagrams::AllpassFilter apf_2_diag;
    HackAudio::Diagrams::AllpassFilter apf_3_diag;

    SchroederReverb()
    {

        comb_1_label.setPlaceholder("FBCF");
        comb_1_label.setBounds(65, -125, 64, 48);
        comb_2_label.setPlaceholder("FBCF");
        comb_2_label.setBounds(150, -90, 64, 48);
        comb_3_label.setPlaceholder("FBCF");
        comb_3_label.setBounds(65, -55, 64, 48);
        comb_4_label.setPlaceholder("FBCF");
        comb_4_label.setBounds(150, -20, 64, 48);
        comb_5_label.setPlaceholder("FBCF");
        comb_5_label.setBounds(65, 20, 64, 48);
        comb_6_label.setPlaceholder("FBCF");
        comb_6_label.setBounds(150, 55, 64, 48);
        comb_7_label.setPlaceholder("FBCF");
        comb_7_label.setBounds(65, 90, 64, 48);
        comb_8_label.setPlaceholder("FBCF");
        comb_8_label.setBounds(150, 125, 64, 48);

        comb_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        comb_sum.setBounds(300, 10, 30, 30);

        apf_1_label.setPlaceholder("APF");
        apf_1_label.setBounds(384, 0, 64, 48);
        apf_2_label.setPlaceholder("APF");
        apf_2_label.setBounds(512, 0, 64, 48);
        apf_3_label.setPlaceholder("APF");
        apf_3_label.setBounds(640, 0, 64, 48);

        addDiagramInput(comb_1_label);
        addDiagramInput(comb_2_label);
        addDiagramInput(comb_3_label);
        addDiagramInput(comb_4_label);
        addDiagramInput(comb_5_label);
        addDiagramInput(comb_6_label);
        addDiagramInput(comb_7_label);
        addDiagramInput(comb_8_label);

        connect(comb_1_label, comb_sum);
        connect(comb_2_label, comb_sum);
        connect(comb_3_label, comb_sum);
        connect(comb_4_label, comb_sum);
        connect(comb_5_label, comb_sum);
        connect(comb_6_label, comb_sum);
        connect(comb_7_label, comb_sum);
        connect(comb_8_label, comb_sum);

        connect(comb_sum, apf_1_label);
        connect(apf_1_label, apf_2_label);
        connect(apf_2_label, apf_3_label);

        addDiagramOutput(apf_3_label);

        setSubDiagram(comb_1_label, comb_1_diag);
        setSubDiagram(comb_2_label, comb_2_diag);
        setSubDiagram(comb_3_label, comb_3_diag);
        setSubDiagram(comb_4_label, comb_4_diag);
        setSubDiagram(comb_5_label, comb_5_diag);
        setSubDiagram(comb_6_label, comb_6_diag);
        setSubDiagram(comb_7_label, comb_7_diag);
        setSubDiagram(comb_8_label, comb_8_diag);

        setSubDiagram(apf_1_label, apf_1_diag);
        setSubDiagram(apf_2_label, apf_2_diag);
        setSubDiagram(apf_3_label, apf_3_diag);

        setName("Schroeder Reverb");

    }

};

#endif
