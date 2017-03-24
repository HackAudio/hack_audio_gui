#ifndef HACK_AUDIO_DIAGRAM_H
#define HACK_AUDIO_DIAGRAM_H

namespace HackAudio
{

class Diagram : public juce::Component
{
public:

    Diagram();
    ~Diagram();

    void connect(juce::Component* source, juce::Component* destination);

private:

    void childrenChanged() override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    juce::HashMap<juce::Component*, juce::Array<juce::Component*>> connections;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diagram)

};

}

#endif
