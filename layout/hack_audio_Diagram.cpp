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

        juce::Array<juce::Component*> newArray = connections[source];

        newArray.addIfNotAlreadyThere(destination);

        connections.set(source, newArray);

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

    g.setColour(HackAudio::Colours::Cyan);

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> destinations = it.getValue();

        for (int i = 0; i < destinations.size(); ++i)
        {

            juce::Component* destination = destinations[i];

            int x1 = source->getWidth();
            int y1 = source->getY() + source->getHeight() / 2;

            int x2 = destination->getX();
            int y2 = destination->getY() + destination->getHeight() / 2;

            juce::Path p;
            p.startNewSubPath(x1, y1);
            p.cubicTo(x2, y1, x1, y2, x2, y2);

            g.strokePath(p, juce::PathStrokeType(4, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        }

    }

}

void HackAudio::Diagram::resized()
{

}
