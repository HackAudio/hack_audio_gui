#ifndef HACK_AUDIO_DIAGRAM_H
#define HACK_AUDIO_DIAGRAM_H

namespace HackAudio
{

/**

 A class that displays labels as a signal flow diagram

*/

class Diagram : private juce::Component,
                private juce::ComponentListener
{

    friend class Viewport;

public:

    Diagram();
    ~Diagram();

    /**
     Designates a component that represents an audio input source of the diagram
    */
    void addInput(juce::Component* component);

    /**
     Removes a component previously designated as an input
    */
    void removeInput(juce::Component* component);

    /**
     Returns an array of current input components
    */
    juce::Array<juce::Component*> getInputs();

    /**
     Designates a component that represents an audio output source of the diagram
    */
    void addOutput(juce::Component* component);

    /**
     Removes a component previously designated as an output
    */
    void removeOutput(juce::Component* component);

    /**
     Returns an array of current output components
    */
    juce::Array<juce::Component*> getOutputs();

    /**
     Adds two components as children and draws a connection between them
     
     @parameter source  the component outputting a connection
     @parameter destination     the component accepting a connection
    */
    void connect(juce::Component* source, juce::Component* destination);

    /**
     Adds a source and destination components as children and draws connections from source to all destinations

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

    /**
     Toggles the state of a connection between source and destination
     
     @parameter source  the component outputting a connection
     @parameter destination  the component to break or receive a connection from source
    */
    void toggle(juce::Component* source, juce::Component* destination);

    /**
     Swaps the state of connections between source, a connected component, and a disconnected component. This method does nothing if both destinations are connected to source or disconnected from source.
     
     @parameter source  the component outputting a connection
     @parameter destinationOne  a component to break or receive a connection from source
     @parameter destinationTwo  a component to break or receive a connection from source
    */
    void swap(juce::Component* source, juce::Component* destinationOne, juce::Component* destinationTwo);

    /**
     Breaks a connection between two components to route it to a new destination. This method only creates the new connection if the connection between source and oldDestination exists
     
     @parameter source  the component outputting a connection
     @parameter oldDestination  the previous component receiving the connection
     @parameter newDestination  the component to reroute the connection to
    */
    void reroute(juce::Component* source, juce::Component* oldDestination, juce::Component* newDestination);


private:

    void updateSize();

    void updateConnections();

    void childrenChanged() override;

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void parentHierarchyChanged() override;

    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool moveGuard;

    juce::Array<juce::Component*> inputComponents;
    juce::Array<juce::Component*> outputComponents;

    juce::HashMap<juce::Component*, juce::Array<juce::Component*>> connections;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diagram)

};

}

#endif
