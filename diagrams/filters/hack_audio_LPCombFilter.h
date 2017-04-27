#ifndef HACK_AUDIO_LPCOMBFILTER_H
#define HACK_AUDIO_LPCOMBFILTER_H

struct LPCombFilter : public HackAudio::Diagram
{
    
    HackAudio::Label delay1;
    HackAudio::Label fb_gain1;
    HackAudio::Diagram::Junction sum1;
    HackAudio::Diagram::Junction mult1;
    HackAudio::Diagram::Junction fb_node1;
    
    HackAudio::Diagram::Junction sum2;
    HackAudio::Diagram::Junction mult2;
    HackAudio::Label delay2;
    HackAudio::Label fb_gain2;
    HackAudio::Diagram::Junction fb_node2;
    
    //

    LPCombFilter()
    {
        
        sum1.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum1.setBounds(250,7.5,30,30);
        mult1.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mult1.setBounds(300,-64,30,30);
        delay1.setPlaceholder("Delay");
        delay1.setBounds(400,-7.5,96,64);
        fb_gain1.setPlaceholder("Gain");
        fb_gain1.setBounds(200,-128,64,48);
        fb_node1.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb_node1.setBounds(550,17,15,15);
        
        sum2.setSymbol(HackAudio::Diagram::Junction::Symbol::Add);
        sum2.setBounds(500,-64,30,30);
        mult2.setSymbol(HackAudio::Diagram::Junction::Symbol::Multiply);
        mult2.setBounds(440,-125,30,30);
        delay2.setPlaceholder("z^-1");
        delay2.setBounds(400,-72,64,48);
        fb_gain2.setPlaceholder("Gain");
        fb_gain2.setBounds(500,-160,48,36);
        fb_node2.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        fb_node2.setBounds(350,-56.5,15,15);
        
        addDiagramInput(sum1);
        connect(sum1,delay1,HackAudio::Diagram::Junction::Horizontal);
        connect(fb_node1,sum2);
        connect(fb_gain1,mult1);
        connect(sum2,delay2);
        connect(delay2,mult1);
        connect(fb_node2,mult2);
        connect(fb_gain2,mult2);
        connect(mult2,sum2,HackAudio::Diagram::Junction::Horizontal);
        connect(mult1,sum1,HackAudio::Diagram::Junction::Horizontal);
        
        addDiagramOutput(delay1);

        setName("Low-Pass, Feed-Back Comb Filter");

    }

};

#endif
