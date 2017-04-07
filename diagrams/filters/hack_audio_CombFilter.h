#ifndef HACK_AUDIO_COMBFILTER_H
#define HACK_AUDIO_COMBFILTER_H

struct CombFilter : public HackAudio::Diagram
{

    HackAudio::Label delay;
    HackAudio::Label gain;

    HackAudio::Diagram::Junction delayLine;
    HackAudio::Diagram::Junction sumJunction;

    CombFilter()
    {

        delayLine.setBounds(0, 0, 24, 24);
        delay.setBounds(64, -96, 64, 64);
        gain.setBounds(192, -96, 64, 64);
        sumJunction.setBounds(320, 0, 24, 24);

        delay.setText("Delay", juce::dontSendNotification);

        gain.setPlaceholder("Gain");

        sumJunction.setSymbol(HackAudio::Diagram::Junction::Add);

        addDiagramInput(delayLine);

        connect(delayLine, delay);
        connect(delayLine, sumJunction);
        connect(delay, gain);
        connect(gain, sumJunction);

        addDiagramOutput(sumJunction);

    }

};

#endif
