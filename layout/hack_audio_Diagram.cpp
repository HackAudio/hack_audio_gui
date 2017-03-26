#include "hack_audio_Diagram.h"

HackAudio::Diagram::Diagram()
{
    setInterceptsMouseClicks(false, false);
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

    repaint();

}

void HackAudio::Diagram::connect(juce::Component* source, juce::Array<juce::Component*> destinations)
{

    assert(getIndexOfChildComponent(source) != -1);

    for (int i = 0; i < destinations.size(); ++i)
    {
        assert(getIndexOfChildComponent(destinations[i]) != -1);
    }

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        for (int i = 0; i < destinations.size(); ++i)
        {

            newArray.addIfNotAlreadyThere(destinations[i]);

        }

        connections.set(source, newArray);

    }
    else
    {

        connections.set(source, destinations);

    }

    repaint();

}

void HackAudio::Diagram::disconnect(juce::Component* source, juce::Component* destination)
{

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        newArray.removeFirstMatchingValue(destination);

        connections.set(source, newArray);

        repaint();

    }
    else
    {

        return;

    }

}

void HackAudio::Diagram::disconnectInputs(juce::Component* component)
{

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> newArray = it.getValue();

        if (newArray.removeAllInstancesOf(component))
        {

            connections.set(source, newArray);

        }

    }

    repaint();

}

void HackAudio::Diagram::disconnectOutputs(juce::Component* component)
{

    if (connections.contains(component))
    {

        connections.remove(component);
        repaint();

    }
    else
    {

        return;

    }

}

void HackAudio::Diagram::toggle(juce::Component *source, juce::Component *destination)
{

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        (newArray.contains(destination)) ? newArray.removeFirstMatchingValue(destination) : newArray.add(destination);

        connections.set(source, newArray);

        repaint();

    }
    else
    {

        return;

    }

}

void HackAudio::Diagram::swap(juce::Component *source, juce::Component *destinationOne, juce::Component *destinationTwo)
{

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        bool stateOne = newArray.contains(destinationOne);
        bool stateTwo = newArray.contains(destinationTwo);

        if (stateOne != stateTwo)
        {

            (stateOne) ? newArray.removeFirstMatchingValue(destinationOne) : newArray.add(destinationOne);
            (stateTwo) ? newArray.removeFirstMatchingValue(destinationTwo) : newArray.add(destinationTwo);

            connections.set(source, newArray);

            repaint();

        }
        else
        {

            return;

        }

    }
    else
    {

        return;

    }

}

void HackAudio::Diagram::reroute(juce::Component *source, juce::Component *oldDestination, juce::Component *newDestination)
{

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        if (newArray.removeAllInstancesOf(oldDestination))
        {

            newArray.addIfNotAlreadyThere(newDestination);

            connections.set(source, newArray);

            repaint();

        }
        else
        {
            return;
        }

    }
    else
    {

        return;

    }

}

void HackAudio::Diagram::childrenChanged()
{
    // Delete all connections for removed children
}

void HackAudio::Diagram::paintOverChildren(juce::Graphics& g)
{

    juce::Array<juce::Path> paths;

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> destinations = it.getValue();

        for (int i = 0; i < destinations.size(); ++i)
        {

            juce::Component* destination = destinations[i];

            int x1 = source->getX() + source->getWidth();
            int y1 = source->getY() + source->getHeight() / 2;

            int x2 = destination->getX();
            int y2 = destination->getY() + destination->getHeight() / 2;

            g.setColour(HackAudio::Colours::Gray);
            g.fillEllipse(x1 - 8, y1 - 8, 16, 16);
            g.setColour(HackAudio::Colours::Black);
            g.drawEllipse(x1 - 8, y1 - 8, 16, 16, 4);

            g.setColour(HackAudio::Colours::Gray);
            g.fillEllipse(x2 - 8, y2 - 8, 16, 16);
            g.setColour(HackAudio::Colours::Black);
            g.drawEllipse(x2 - 8, y2 - 8, 16, 16, 4);

            juce::Path p;
            p.startNewSubPath(x1, y1);
            p.cubicTo(x2, y1, x1, y2, x2, y2);

            paths.add(p);

        }

    }

    g.setColour(HackAudio::Colours::Gray);

    for (int i = 0; i < paths.size(); ++i)
    {
        g.strokePath(paths[i], juce::PathStrokeType(4));
    }

}

void HackAudio::Diagram::resized()
{

}
