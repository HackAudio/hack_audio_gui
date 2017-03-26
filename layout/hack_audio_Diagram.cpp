#include "hack_audio_Diagram.h"

HackAudio::Diagram::Diagram()
{

    setInterceptsMouseClicks(false, false);

}

HackAudio::Diagram::~Diagram()
{

}

void HackAudio::Diagram::setInput(juce::Component *component)
{

    assert(getIndexOfChildComponent(component) != -1);

    inputComponent = component;

}

juce::Component* HackAudio::Diagram::getInput()
{
    return inputComponent;
}

void HackAudio::Diagram::setOutput(juce::Component* component)
{

    assert(getIndexOfChildComponent(component) != -1);

    outputComponent = component;
}

juce::Component* HackAudio::Diagram::getOutput()
{
    return outputComponent;
}

void HackAudio::Diagram::connect(juce::Component* source, juce::Component* destination)
{

    addAndMakeVisible(source);
    addAndMakeVisible(destination);

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

    addAndMakeVisible(source);

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        for (int i = 0; i < destinations.size(); ++i)
        {

            addAndMakeVisible(destinations[i]);

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

void HackAudio::Diagram::updateSize()
{

    if (!isVisible()) { return; }

    int minX = 0x0FFFFFFF;
    int minY = 0x0FFFFFFF;
    int maxX = 0xF0000000;
    int maxY = 0xF0000000;

    for (int i = 0; i < getNumChildComponents(); ++i)
    {

        juce::Component* c = getChildComponent(i);
        minX = std::min(c->getX(), minX);
        minY = std::min(c->getY(), minY);

        maxX = std::max(c->getRight(), maxX);
        maxY = std::max(c->getBottom(), maxY);

    }

    if (minX > 0 || minY > 0)
    {

        for (int i = 0; i < getNumChildComponents(); ++i)
        {

            juce::Component* c = getChildComponent(i);

            c->setTopLeftPosition(c->getX() - minX, c->getY() - minY);

        }

    }

    setBounds(getX() + minX, getY() + minY, maxX - minX, maxY - minY);

}

void HackAudio::Diagram::updateConnections()
{

    bool orphanedConnections = false;

    if (getIndexOfChildComponent(inputComponent)  == -1) { inputComponent = nullptr;  }
    if (getIndexOfChildComponent(outputComponent) == -1) { outputComponent = nullptr; }

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> destinations = it.getValue();

        if (getIndexOfChildComponent(source) == -1)
        {

            connections.remove(source);

            orphanedConnections = true;

            continue;

        }
        else
        {

            for (int i = 0; i < destinations.size(); ++i)
            {

                juce::Component* c = destinations[i];

                if (getIndexOfChildComponent(c) == -1)
                {

                    destinations.remove(i);

                    orphanedConnections = true;

                }

            }

        }
        
        if (orphanedConnections)
        {
            connections.set(source, destinations);
        }
        
    }
    
    if (orphanedConnections)
    {
        repaint();
    }

}

void HackAudio::Diagram::childrenChanged()
{

    updateSize();
    updateConnections();

}

void HackAudio::Diagram::parentHierarchyChanged()
{

    updateSize();
    updateConnections();

}

void HackAudio::Diagram::paint(juce::Graphics& g)
{

#ifdef JUCE_DEBUG
    g.fillAll(juce::Colours::paleturquoise);
#endif

}

void HackAudio::Diagram::paintOverChildren(juce::Graphics& g)
{

    juce::Array<juce::Path> paths;

    juce::Array<juce::Component*> inputConnectedComponents;
    juce::Array<juce::Component*> outputConnectedComponents;

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        if (!source->isVisible()) { repaint(); continue; }

        juce::Array<juce::Component*> destinations = it.getValue();

        for (int i = 0; i < destinations.size(); ++i)
        {

            juce::Component* destination = destinations[i];

            if (!destination->isVisible()) { repaint(); continue; }

            int x1 = source->getX() + source->getWidth();
            int y1 = source->getY() + source->getHeight() / 2;

            int x2 = destination->getX();
            int y2 = destination->getY() + destination->getHeight() / 2;

            if (!outputConnectedComponents.contains(source))
            {

                g.setColour(HackAudio::Colours::Gray);
                g.fillEllipse(x1 - 8, y1 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Black);
                g.drawEllipse(x1 - 8, y1 - 8, 16, 16, 4);

            }

            if (!inputConnectedComponents.contains(destination))
            {

                g.setColour(HackAudio::Colours::Gray);
                g.fillEllipse(x2 - 8, y2 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Black);
                g.drawEllipse(x2 - 8, y2 - 8, 16, 16, 4);

            }

            juce::Path p;
            p.startNewSubPath(x1, y1);
            p.cubicTo(x2, y1, x1, y2, x2, y2);

            paths.add(p);

            outputConnectedComponents.add(source);
            inputConnectedComponents.add(destination);

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
