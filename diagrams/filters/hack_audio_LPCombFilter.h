#ifndef HACK_AUDIO_LPCOMBFILTER_H
#define HACK_AUDIO_LPCOMBFILTER_H

struct LPCombFilter : public HackAudio::Diagram
{
    
    HackAudio::Label delay_1;
    HackAudio::Label fb_1_gain;
    HackAudio::Diagram::Junction sum_1;
    HackAudio::Diagram::Junction mult_1;
    HackAudio::Diagram::Junction fb_1_node;
    
    HackAudio::Diagram::Junction sum_2;
    HackAudio::Diagram::Junction mult_2;
    HackAudio::Label delay_2;
    HackAudio::Label fb_gain2;
    HackAudio::Diagram::Junction fb_2_node;
    
    HackAudio::Diagrams::Moddelay moddelay_diag;

    LPCombFilter()
    {
        
        sum_1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum_1.setBounds(250,0,30,30);
        mult_1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mult_1.setBounds(300,-64,30,30);
        delay_1.setPlaceholder("Delay");
        delay_1.setBounds(400,-17,96,64);
        fb_1_gain.setPlaceholder("Gain");
        fb_1_gain.setBounds(200,-128,64,48);
        fb_1_node.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb_1_node.setBounds(550,8,15,15);
        
        sum_2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum_2.setBounds(500,-64,30,30);
        mult_2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mult_2.setBounds(440,-125,30,30);
        delay_2.setPlaceholder("z^{-1}");
        delay_2.setBounds(400,-72,64,48);

        fb_gain2.setPlaceholder("Gain");
        fb_gain2.setBounds(500,-160,64,48);
        fb_2_node.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb_2_node.setBounds(350,-56.5,15,15);
        
        addDiagramInput(sum_1);

        connect(sum_1,delay_1,HackAudio::Diagram::Junction::Horizontal);
        connect(delay_1, fb_1_node);
        connect(fb_1_node,sum_2);
        connect(fb_1_gain,mult_1);
        connect(sum_2,delay_2);
        connect(delay_2,mult_1);
        connect(fb_2_node,mult_2);
        connect(fb_gain2,mult_2);
        connect(mult_2,sum_2,HackAudio::Diagram::Junction::Horizontal);
        connect(mult_1,sum_1,HackAudio::Diagram::Junction::Horizontal);
        
        addDiagramOutput(fb_1_node);

        setName("Low-Pass, Feed-Back Comb Filter");

        setSubDiagram(delay_1, moddelay_diag);
    }

};

#endif
