#ifndef HACK_AUDIO_ALLPASS_H
#define HACK_AUDIO_ALLPASS_H

struct AllpassFilter : public HackAudio::Diagram
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

    AllpassFilter()
    {

        main_delay.setPlaceholder("Delay");
        main_delay.setBounds(325, -8, 96, 64);
        main_sum_1.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_1.setBounds(200, 9, 30, 30);
        main_sum_2.setSymbol(HackAudio::Diagram::Junction::Add);
        main_sum_2.setBounds(550, 9, 30, 30);

        fb_gain.setPlaceholder("Gain");
        fb_gain.setBounds(175, -100, 64, 48);
        fb_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        fb_mult.setBounds(30, -50, 30, 30);
        fb_node.setSymbol(HackAudio::Diagram::Junction::None);
        fb_node.setBounds(475, 15, 15, 15);

        ff_gain.setPlaceholder("Gain");
        ff_gain.setBounds(540, 100, 64, 48);
        ff_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        ff_mult.setBounds(450, 70, 30, 30);
        ff_node.setSymbol(HackAudio::Diagram::Junction::None);
        ff_node.setBounds(250, 15, 15, 15);

        addDiagramInput(main_sum_1);

        connect(main_sum_1, main_delay);
        connect(main_delay, main_sum_2);

        connect(fb_node, fb_mult);
        connect(fb_gain, fb_mult);
        connect(fb_mult, main_sum_1);

        connect(ff_node, ff_mult);
        connect(ff_gain, ff_mult);
        connect(ff_mult, main_sum_2, HackAudio::Diagram::Junction::Horizontal);

        addDiagramOutput(main_sum_2);

        setName("All-Pass Filter");

    }

};

#endif
