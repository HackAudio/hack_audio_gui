#ifndef HACK_AUDIO_COMBFILTER_H
#define HACK_AUDIO_COMBFILTER_H

struct CombFilter : public HackAudio::Diagram
{

    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_sum;
    HackAudio::Diagram::Junction main_mult;

    HackAudio::Label fb_gain;
    HackAudio::Diagram::Junction fb_node;

    CombFilter()
    {

        main_sum.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum.setBounds(250, 10, 30, 30);
        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(300, -64, 30, 30);
        main_delay.setPlaceholder("Delay");
        main_delay.setBounds(400, -7.5, 96, 64);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(350, -128, 64, 48);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(550, 17, 15, 15);

        addDiagramInput(main_sum);

        connect(main_sum, main_delay);
        connect(fb_node, main_mult);
        connect(fb_gain, main_mult);
        connect(main_mult, main_sum, HackAudio::Diagram::Junction::Horizontal);
        connect(main_delay, fb_node);

        addDiagramOutput(fb_node);

        setName("Feed-Back Comb Filter");

    }

};

#endif
