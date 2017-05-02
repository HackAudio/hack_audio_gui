#ifndef HACK_AUDIO_FDNREVERB_H
#define HACK_AUDIO_FDNREVERB_H

struct FDNReverb : public HackAudio::Diagram
{
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction sum3;
    HackAudio::Diagram::Junction sum4;
    
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

        delay_1_label.setPlaceholder("Delay");
        delay_1_label.setBounds(310,-108,64,40);
        delay_2_label.setPlaceholder("Delay");
        delay_2_label.setBounds(250,-72,64,40);
        delay_3_label.setPlaceholder("Delay");
        delay_3_label.setBounds(190,-36,64,40);
        delay_4_label.setPlaceholder("Delay");
        delay_4_label.setBounds(130,0,64,40);
        
        matrix_label.setPlaceholder("\\array Feed-back &// Matrix \\end");
        matrix_label.setBounds(210,43,130,105);
        
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
        
        connect(matrix_label,sum1);
        connect(matrix_label,sum2);
        connect(matrix_label,sum3);
        connect(matrix_label,sum4);
        
        addDiagramOutput(sumOut);

        setName("Feed-back Delay Network Reverb");

    }

};

#endif
