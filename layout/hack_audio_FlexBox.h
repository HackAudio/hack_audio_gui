#ifndef HACK_AUDIO_FLEXBOX_H
#define HACK_AUDIO_FLEXBOX_H

namespace HackAudio
{

class FlexBox : public juce::FlexBox,
                private juce::Component
{
public:

    FlexBox();
    ~FlexBox();

    void addComponent(juce::Component* component);
    void removeComponent(juce::Component* component);
    void applyBounds(juce::Rectangle<int> bounds);

private:

    void applyLayout();

};

}

#endif
