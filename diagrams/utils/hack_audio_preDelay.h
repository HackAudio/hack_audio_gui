#ifndef HACK_AUDIO_PREDELAY_H
#define HACK_AUDIO_PREDELAY_H

struct Predelay : public HackAudio::Diagram
{

    HackAudio::Label main_ms;
    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_node;

    Predelay()
    {

        setName("Pre-delay");

        main_ms.setPlaceholder("Milliseconds");
        main_ms.setBounds(0, -80, 96, 64);

        main_delay.setPlaceholder("Samples of\nDelay");
        main_delay.setBounds(150, 0, 128, 64);
        
        main_node.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        main_node.setBounds(214,0,0,0);

        addDiagramInput(main_delay);

        connect(main_ms, main_node);

        addDiagramOutput(main_delay);

    }

};

#endif

