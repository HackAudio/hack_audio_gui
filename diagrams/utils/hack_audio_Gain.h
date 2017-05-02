#ifndef HACK_AUDIO_GAIN_H
#define HACK_AUDIO_GAIN_H

struct Gain : public HackAudio::Diagram
{

    HackAudio::Label main_db;
    HackAudio::Label main_linear;
    HackAudio::Diagram::Junction main_mult;

    Gain()
    {

        setName("Gain");

        main_db.setPlaceholder("Decibel");
        main_db.setBounds(0, -64, 96, 64);

        main_linear.setPlaceholder("10^{(dB/20)}");
        main_linear.setBounds(256, -64, 128, 64);

        main_mult.setSymbol(HackAudio::Diagram::Junction::Multiply);
        main_mult.setBounds(512, 64, 40, 40);

        addDiagramInput(main_mult);

        connect(main_db, main_linear);
        connect(main_linear, main_mult);

        addDiagramOutput(main_mult);

    }

};

#endif

