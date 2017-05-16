#ifndef HACK_AUDIO_MODDELAY_H
#define HACK_AUDIO_MODDELAY_H

struct Moddelay : public HackAudio::Diagram
{

    HackAudio::Label main_lfo;
    HackAudio::Label main_delay;
    HackAudio::Diagram::Junction main_node;

    Moddelay()
    {

        setName("Modulated Delay");

        main_lfo.setPlaceholder("LFO");
        main_lfo.setBounds(0, -80, 96, 64);

        main_delay.setPlaceholder("z^{-0}");
        main_delay.setBounds(150, 0, 128, 64);
        
        main_node.setSymbol(HackAudio::Diagram::Junction::Symbol::None);
        main_node.setBounds(214,0,0,0);

        addDiagramInput(main_delay);

        connect(main_lfo, main_node);

        addDiagramOutput(main_delay);

    }

};

#endif

