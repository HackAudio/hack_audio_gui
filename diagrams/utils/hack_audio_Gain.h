#ifndef HACK_AUDIO_GAIN_H
#define HACK_AUDIO_GAIN_H

struct Gain : public HackAudio::Diagram
{

    HackAudio::Label dBGain;
    HackAudio::Label linearGain;

    Gain()
    {

        dBGain.setBounds(0, 0, 64, 64);
        linearGain.setBounds(128, 0, 64, 64);

        addDiagramInput(dBGain);
        connect(dBGain, linearGain);
        addDiagramOutput(dBGain);

    }

};

#endif

