#ifndef HACK_AUDIO_GARDNERMEDIUMREVERB_H
#define HACK_AUDIO_GARDNERMEDIUMREVERB_H

struct GardnermediumReverb : public HackAudio::Diagram
{

    HackAudio::Label napf_1_label;
    HackAudio::Label napf_2_label;
    HackAudio::Label apf_label;
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
    HackAudio::Diagrams::AllpassFilter apf_diag;
    HackAudio::Diagrams::NestedtwoFilter napf_1_diag;
    HackAudio::Diagrams::NestedallpassFilter napf_2_diag;

    GardnermediumReverb()
    {

        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(0,8,30,30);
        
        napf_1_label.setPlaceholder("2NAPF");
        napf_1_label.setBounds(65,0,60,48);
        
        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(165,0,60,48);
        
        apf_label.setPlaceholder("APF");
        apf_label.setBounds(250,0,60,48);
        
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(340,0,60,48);
        
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(440,0,60,48);
        
        napf_2_label.setPlaceholder("NAPF");
        napf_2_label.setBounds(550,0,60,48);
        
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(440,95,60,48);
        
        lpf_label.setPlaceholder("LPF");
        lpf_label.setBounds(256,95,60,48);
        
        gain_1_label.setPlaceholder("Gain");
        gain_1_label.setBounds(335,-48,50,30);
        
        gain_2_label.setPlaceholder("Gain");
        gain_2_label.setBounds(550,-48,50,30);
        
        gain_3_label.setPlaceholder("Gain");
        gain_3_label.setBounds(100,64,50,30);
        
        // Output
        node1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node1.setBounds(415,18,12,12);
        
        node2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node2.setBounds(637,64,0,0);
        
        mul1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul1.setBounds(405,-48,30,30);
        
        mul2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul2.setBounds(620,-48,30,30);
        
        mul3.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul3.setBounds(150,104,30,30);
        
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(660,-96,30,30);
        
        sum3.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum3.setBounds(445,-96,30,30);
        
        gain_0_label.setPlaceholder("Gain");
        gain_0_label.setBounds(55,-96,50,30);
        mul0.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mul0.setBounds(130,-96,30,30);
        node0.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        node0.setBounds(140,18,12,12); 
        

        addDiagramInput(sum1);
        connect(sum1,napf_1_label);
        connect(napf_1_label,delay_1_label);
        connect(delay_1_label,apf_label);
        connect(apf_label,delay_2_label);
        connect(delay_2_label,delay_3_label);
        connect(delay_3_label,napf_2_label);
        
        connect(napf_2_label,mul2);
        connect(napf_2_label,node2);
        connect(node2,delay_4_label);
        connect(delay_4_label,lpf_label);
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
        
        
        setName("Gardner Medium Reverb");
        
        setSubDiagram(lpf_label, lpf_diag);
        setSubDiagram(apf_label, apf_diag);
        setSubDiagram(napf_1_label, napf_1_diag);
        setSubDiagram(napf_2_label, napf_2_diag);
    }

};

#endif
