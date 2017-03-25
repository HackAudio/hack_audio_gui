#ifndef HACK_AUDIO_DIAGRAM_H
#define HACK_AUDIO_DIAGRAM_H

namespace HackAudio
{

/**

 A class that displays labels as a signal flow diagram

*/

class Diagram : public juce::Component
{
public:

    Diagram();
    ~Diagram();

    /**
     Draw a connection between two components
    */
    void connect(juce::Component* source, juce::Component* destination);

private:

    void childrenChanged() override;

    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    juce::HashMap<juce::Component*, juce::Array<juce::Component*>> connections;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diagram)

};

}

#endif
