#ifndef HACK_AUDIO_NESTEDALLPASS_H
#define HACK_AUDIO_NESTEDALLPASS_H

struct NestedallpassFilter : public HackAudio::Diagram
{

    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_sum_1;
    HackAudio::Diagram::Junction main_sum_2;

    HackAudio::Label fb_gain;
    HackAudio::Diagram::Junction fb_mult;
    HackAudio::Diagram::Junction fb_node;

    HackAudio::Label ff_gain;
    HackAudio::Diagram::Junction ff_mult;
    HackAudio::Diagram::Junction ff_node;
    
    HackAudio::Label apf_label;
    
    HackAudio::Diagrams::AllpassFilter apf_diag;

    NestedallpassFilter()
    {
        apf_label.setPlaceholder("APF");
        apf_label.setBounds(445, -8, 96, 64);
        
        main_delay.setPlaceholder("Delay");
        main_delay.setBounds(315, -8, 96, 64);
        main_sum_1.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_1.setBounds(200, 9, 30, 30);
        main_sum_2.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_2.setBounds(650, 9, 30, 30);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(130, -100, 64, 48);
        fb_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        fb_mult.setBounds(275, -50, 30, 30);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(575, 15, 15, 15);

        ff_gain.setPlaceholder("Gain");
        ff_gain.setBounds(640, 100, 64, 48);
        ff_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_mult.setBounds(550, 70, 30, 30);
        ff_node.setSymbol(HackAudio::Diagram::Junction::None);
        ff_node.setBounds(250, 15, 15, 15);

        addDiagramInput(main_sum_1);

        connect(main_sum_1, main_delay);
        connect(main_delay, apf_label);
        connect(apf_label, main_sum_2);

        connect(fb_node, fb_mult);
        connect(fb_gain, fb_mult);
        connect(fb_mult, main_sum_1,HackAudio::Diagram::Junction::Horizontal);

        connect(ff_node, ff_mult);
        connect(ff_gain, ff_mult);
        connect(ff_mult, main_sum_2, HackAudio::Diagram::Junction::Horizontal);

        addDiagramOutput(main_sum_2);

        setName("Nested All-Pass Filter");
        
        setSubDiagram(apf_label, apf_diag);

    }

};

#endif
