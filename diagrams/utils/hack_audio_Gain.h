#ifndef HACK_AUDIO_GAIN_H
#define HACK_AUDIO_GAIN_H

struct Gain : public HackAudio::Diagram
{

    HackAudio::Label testOne;
    HackAudio::Label testTwo;

    Gain()
    {

        testOne.setBounds(0, 0, 64, 64);
        testTwo.setBounds(128, 0, 64, 64);

        addDiagramInput(testOne);
        connect(testOne, testTwo);
        addDiagramOutput(testTwo);

    }

    ~Gain() {}

};

#endif

