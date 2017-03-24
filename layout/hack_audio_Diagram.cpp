#include "hack_audio_Diagram.h"

HackAudio::Diagram::Diagram()
{

}

HackAudio::Diagram::~Diagram()
{

}

void HackAudio::Diagram::connect(juce::Component* source, juce::Component* destination)
{

    assert(getIndexOfChildComponent(source)      != -1);
    assert(getIndexOfChildComponent(destination) != -1);

    if(connections.contains(source))
    {
        connections[source].addIfNotAlreadyThere(destination);
    }
    else
    {

        juce::Array<juce::Component*> newArray;

        newArray.add(destination);

        connections.set(source, newArray);
    }

}

void HackAudio::Diagram::childrenChanged()
{
    // Delete all connections for removed children
}

void HackAudio::Diagram::paint(juce::Graphics& g)
{
    // Draw grid?
}

void HackAudio::Diagram::paintOverChildren(juce::Graphics& g)
{
    // Draw lines, connector nodes

    g.setColour(HackAudio::Colours::Cyan);

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> destinations = it.getValue();

        for (int i = 0; i < destinations.size(); ++i)
        {

            juce::Component* destination = destinations[i];

            g.drawLine(source->getX(), source->getY(), destination->getX(), destination->getY(), 4);

        }

    }

}

void HackAudio::Diagram::resized()
{

}
