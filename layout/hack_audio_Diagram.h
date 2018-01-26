/* Copyright (C) 2017 by Antonio Lassandro, HackAudio LLC
 *
 * hack_audio_gui is provided under the terms of The MIT License (MIT):
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef HACK_AUDIO_DIAGRAM_H
#define HACK_AUDIO_DIAGRAM_H

namespace HackAudio
{

/**
 A class that displays HackAudio::Labels in a signal flow diagram
*/
class Diagram : public juce::Component,
                private juce::ComponentListener
{

    friend class Viewport;

public:

    /**
     A circular junction for use in HackAudio::Diagrams
    */
    class Junction : public juce::Component
    {

        friend Diagram;

    public:

        Junction();
        ~Junction();

        /**
         The mathematical operation symbol the junction will display
        */
        enum Symbol
        {

            None,           /**< An empty junction, usually used to represent a signal path splitting into multiple paths */
            Add,            /**< An addition junction */
            Subtract,       /**< A subtraction junction */
            Divide,         /**< A dividing junction */
            Multiply,       /**< A multiplying junction */
            Power,          /**< An exponential junction */
            Sqrt,           /**< A square root junction */
            Average         /**< An averaging junction */

        };

        /**
         The direction that the junction should connect from
        */
        enum Direction
        {
            Null,           /**< I forget */
            Auto,           /**< The diagram automatically picks the best direction in which to draw the connection */
            Vertical,       /**< The diagram only draws the connection from either the top or bottom of the junction */
            Horizontal      /**< The diagram only draws the connection from either the left or right of the junction */
        };

        /**
         Sets the junction's displayed symbol
         
         @see Symbol
        */
        void setSymbol(Symbol s);

    private:

        void paint(juce::Graphics& g) override;

        juce::String currentSymbol;

        juce::HashMap<juce::Component*, Direction> outputDirections;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Junction)

    };

    // ==========================================================

    Diagram();
    ~Diagram();

    using juce::Component::setName;
    using juce::Component::getName;

    /**
     Designates a component that represents an audio input source of the diagram
    */
    void addDiagramInput(juce::Component& component);

    /**
     Removes a component previously designated as an input
    */
    void removeDiagramInput(juce::Component& component);

    /**
     Returns an array of current input components
    */
    juce::Array<juce::Component*> getDiagramInputs() const;

    /**
     Designates a component that represents an audio output source of the diagram
    */
    void addDiagramOutput(juce::Component& component);

    /**
     Removes a component previously designated as an output
    */
    void removeDiagramOutput(juce::Component& component);

    /**
     Returns an array of current output components
    */
    juce::Array<juce::Component*> getDiagramOutputs() const;

    /**
     Adds two components as children and draws a connection between them
     
     @param source  the component outputting a connection
     @param destination     the component accepting a connection
    */
    void connect(juce::Component& source, juce::Component& destination);

    /**
     Adds a junction and component as children and draws a connection between them

     @param source  the component outputting a connection
     @param destination     the component accepting a connection
     @param directionFromSource     this is a specific argument to determine which axis the connection should be drawn from when the source is a HackAudio::Diagram::Junction
     */
    void connect(Junction& source, juce::Component& destination, Junction::Direction directionFromSource = Junction::Direction::Auto);

    /**
     Breaks a connection between two components

     @param source  the component outputting a connection
     @param destination     the component accepting a connection
    */
    void disconnect(juce::Component& source, juce::Component& destination);

    /**
     Breaks all incoming connections to the component
    */
    void disconnectInputs(juce::Component& component);

    /**
     Breaks all outgoing connections from the component
    */
    void disconnectOutputs(juce::Component& component);

    /**
     Toggles the state of a connection between source and destination
     
     @param source  the component outputting a connection
     @param destination  the component to break or receive a connection from source
    */
    void toggle(juce::Component& source, juce::Component& destination);

    /**
     Swaps the state of connections between source, a connected component, and a disconnected component. This method does nothing if both destinations are connected to source or disconnected from source.
     
     @param source  the component outputting a connection
     @param destinationOne  a component to break or receive a connection from source
     @param destinationTwo  a component to break or receive a connection from source
    */
    void swap(juce::Component& source, juce::Component& destinationOne, juce::Component& destinationTwo);

    /**
     Breaks a connection between two components to route it to a new destination. This method only creates the new connection if the connection between source and oldDestination exists
     
     @param source  the component outputting a connection
     @param oldDestination  the previous component receiving the connection
     @param newDestination  the component to reroute the connection to
    */
    void reroute(juce::Component& source, juce::Component& oldDestination, juce::Component& newDestination);

    /**
     Designates a diagram to be expanded when double-clicking the source component
     
     @param source  the component to double click to expand the diagram
     @param subDiagram  the diagram to expand
    */
    void setSubDiagram(juce::Component& source, HackAudio::Diagram& subDiagram);

private:

    void updateSize();
    void updateConnections();
    void updateChildren();

    void childrenChanged() override;
    void parentHierarchyChanged() override;

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void paintOverChildren(juce::Graphics& g) override;

    bool moveGuard;

    juce::Array<juce::Component*> inputComponents;
    juce::Array<juce::Component*> outputComponents;

    juce::Path connectionPaths;
    juce::HashMap<juce::Component*, juce::Array<juce::Component*>> connections;

    juce::HashMap<juce::Component*, HackAudio::Diagram*> submap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diagram)

};

}

#endif
