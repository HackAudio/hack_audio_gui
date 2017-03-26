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
     
     @parameter source  the component outputting a connection
     @parameter destination     the component accepting a connection
    */
    void connect(juce::Component* source, juce::Component* destination);

    /**
     Draw a connection between the source and all destination components

     @parameter source  the component outputting connections
     @parameter destinations     the components accepting the connections
     */
    void connect(juce::Component* source, juce::Array<juce::Component*> destinations);

    /**
     Breaks a connection between two components

     @parameter source  the component outputting a connection
     @parameter destination     the component accepting a connection
    */
    void disconnect(juce::Component* source, juce::Component* destination);

    /**
     Breaks all incoming connections to the component
    */
    void disconnectInputs(juce::Component* component);

    /**
     Breaks all outgoing connections from the component
    */
    void disconnectOutputs(juce::Component* component);


private:

    void childrenChanged() override;

    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    juce::HashMap<juce::Component*, juce::Array<juce::Component*>> connections;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diagram)

};

}

#endif
