#include "hack_audio_Diagram.h"

HackAudio::Diagram::Junction::Junction()
{

}

HackAudio::Diagram::Junction::~Junction()
{

}

void HackAudio::Diagram::Junction::setSymbol(HackAudio::Diagram::Junction::Symbol s)
{

    switch (s)
    {
        case HackAudio::Diagram::Junction::Symbol::None:
            currentSymbol = "";
            break;

        case HackAudio::Diagram::Junction::Symbol::Add:
            currentSymbol = "+";
            break;

        case HackAudio::Diagram::Junction::Symbol::Subtract:
            currentSymbol = "-";
            break;

        case HackAudio::Diagram::Junction::Symbol::Divide:
            currentSymbol = "÷";
            break;

        case HackAudio::Diagram::Junction::Symbol::Multiply:
            currentSymbol = "×";
            break;

        case HackAudio::Diagram::Junction::Symbol::Average:
            currentSymbol = "avg";
            break;

        default:
            currentSymbol = "";
            break;
    }

}

void HackAudio::Diagram::Junction::paint(juce::Graphics& g)
{

    int width = getWidth();
    int height = getHeight();

    if (width <= 0 || height <= 0) { return; }

    g.setColour(HackAudio::Colours::Gray);
    g.fillEllipse(0, 0, width, height);

    g.setColour(HackAudio::Colours::White);
    g.drawFittedText(currentSymbol, 0, 0, width, height, juce::Justification::centred, 1);

}

// ============================================================

HackAudio::Diagram::Diagram()
{

    moveGuard = false;

    setBufferedToImage(true);

}

HackAudio::Diagram::~Diagram()
{

}

void HackAudio::Diagram::addDiagramInput(juce::Component *component)
{

    addAndMakeVisible(component);

    inputComponents.addIfNotAlreadyThere(component);

    updateChildren();

    if (getParentComponent())
    {
        getParentComponent()->repaint();
    }

}

void HackAudio::Diagram::removeDiagramInput(juce::Component *component)
{

    inputComponents.removeFirstMatchingValue(component);

    updateChildren();

    if (getParentComponent())
    {
        getParentComponent()->repaint();
    }

}

juce::Array<juce::Component*> HackAudio::Diagram::getDiagramInputs()
{

    return inputComponents;

}

void HackAudio::Diagram::addDiagramOutput(juce::Component* component)
{

    addAndMakeVisible(component);

    outputComponents.addIfNotAlreadyThere(component);

    updateChildren();

    if (getParentComponent())
    {
        getParentComponent()->repaint();
    }

}

void HackAudio::Diagram::removeDiagramOutput(juce::Component* component)
{

    outputComponents.removeFirstMatchingValue(component);

    updateChildren();

    if (getParentComponent())
    {
        getParentComponent()->repaint();
    }

}

juce::Array<juce::Component*> HackAudio::Diagram::getDiagramOutputs()
{

    return outputComponents;
    
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

    updateChildren();

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

    updateChildren();

}

void HackAudio::Diagram::disconnect(juce::Component* source, juce::Component* destination)
{

    if (connections.contains(source))
    {

        juce::Array<juce::Component*> newArray = connections[source];

        newArray.removeFirstMatchingValue(destination);

        connections.set(source, newArray);

        updateChildren();

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

    inputComponents.removeFirstMatchingValue(component);

    updateChildren();

}

void HackAudio::Diagram::disconnectOutputs(juce::Component* component)
{

    outputComponents.removeFirstMatchingValue(component);

    if (connections.contains(component))
    {

        connections.remove(component);

        updateChildren();

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

        updateChildren();

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

void HackAudio::Diagram::setSubDiagram(juce::Component *source, HackAudio::Diagram *subDiagram)
{

    assert(getIndexOfChildComponent(source) != -1);

    submap.set(source, subDiagram);

}

void HackAudio::Diagram::updateSize()
{

    if (!isVisible()) { return; }

    moveGuard = true;

    int minX = 0x0FFFFFFF;
    int minY = 0x0FFFFFFF;
    int maxX = 0xF0000000;
    int maxY = 0xF0000000;

    for (int i = 0; i < getNumChildComponents(); ++i)
    {

        juce::Component* c = getChildComponent(i);

        if (!c->isVisible())
        {
            continue;
        }

        minX = std::min(c->getX(), minX);
        minY = std::min(c->getY(), minY);

        maxX = std::max(c->getRight(), maxX);
        maxY = std::max(c->getBottom(), maxY);

    }

    if (minX != 0 || minY != 0)
    {

        for (int i = 0; i < getNumChildComponents(); ++i)
        {

            juce::Component* c = getChildComponent(i);

            c->setTopLeftPosition(c->getX() - minX, c->getY() - minY);

        }

    }

    if (minX == 0x0FFFFFFFF || minY == 0x0FFFFFFFF || maxX == 0xF0000000 || maxY == 0xF0000000)
    {

        minX = 0;
        minY = 0;
        maxX = 0;
        maxY = 0;

    }

    setBounds(getX() + minX, getY() + minY, maxX - minX, maxY - minY);

    moveGuard = false;

}

void HackAudio::Diagram::updateConnections()
{

    bool orphanedConnections = false;

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        juce::Array<juce::Component*> destinations = it.getValue();

        if (getIndexOfChildComponent(source) == -1)
        {

            if (getDiagramInputs().contains(source)) { removeDiagramInput(source); }
            if (getDiagramOutputs().contains(source)) { removeDiagramOutput(source); }

            source->removeComponentListener(this);

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

                    if (getDiagramInputs().contains(c)) { removeDiagramInput(c); }
                    if (getDiagramOutputs().contains(c)) { removeDiagramOutput(c); }

                    c->removeComponentListener(this);

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

void HackAudio::Diagram::updateChildren()
{

    for (int i = 0; i < getNumChildComponents(); ++i)
    {

        juce::Component* c = getChildComponent(i);

        bool childHasConnections = false;

        if (inputComponents.contains(c) || outputComponents.contains(c))
        {
            childHasConnections = true;
        }

        if (connections.contains(c))
        {
            childHasConnections = true;
        }

        for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
        {

            juce::Array<juce::Component*> arr = it.getValue();

            if (arr.contains(c))
            {
                childHasConnections = true;
            }

        }

        if (!childHasConnections)
        {
            c->setVisible(false);
        }
        else
        {
            c->setVisible(true);
        }

    }

    updateSize();

}

void HackAudio::Diagram::childrenChanged()
{

    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        juce::Component* c = getChildComponent(i);

        c->setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Gray);

        c->addComponentListener(this);
    }

    updateSize();
    updateConnections();

}

void HackAudio::Diagram::parentHierarchyChanged()
{

    updateSize();
    updateConnections();

}

void HackAudio::Diagram::componentMovedOrResized(juce::Component &component, bool wasMoved, bool wasResized)
{

    if (!moveGuard)
    {
        updateSize();
    }
    
}

void HackAudio::Diagram::paintOverChildren(juce::Graphics& g)
{

    juce::Path p;

    for(juce::HashMap<juce::Component*, juce::Array<juce::Component*>>::Iterator it (connections); it.next();)
    {

        juce::Component* source = it.getKey();

        if (!source->isVisible()) { repaint(); continue; }

        juce::Array<juce::Component*> destinations = it.getValue();

        for (int i = 0; i < destinations.size(); ++i)
        {

            juce::Component* destination = destinations[i];

            if (!destination->isVisible()) { repaint(); continue; }

            int x1, y1, x2, y2;

            bool inverseIO = false;

            if (source->getX() < destination->getX())
            {

                x1 = source->getX() + source->getWidth();
                y1 = source->getY() + source->getHeight() / 2;

                x2 = destination->getX();
                y2 = destination->getY() + destination->getHeight() / 2;

            }
            else
            {

                inverseIO = true;

                x1 = source->getX();
                y1 = source->getY() + source->getHeight() / 2;

                x2 = destination->getX() + destination->getWidth();
                y2 = destination->getY() + destination->getHeight() / 2;

            }

            juce::Colour colourOne = (inverseIO) ? HackAudio::Colours::Black : HackAudio::Colours::Gray;
            juce::Colour colourTwo = (inverseIO) ? HackAudio::Colours::Gray : HackAudio::Colours::Black;

            if (inverseIO)
            {

                juce::Path temp;
                temp.startNewSubPath(x1, y1);
                temp.cubicTo(x2, y1, x1, y2, x2, y2);
                g.setColour(HackAudio::Colours::Gray);
                g.strokePath(temp, juce::PathStrokeType(4));
                
            }

            if (!dynamic_cast<Junction*>(source))
            {

                g.setColour(colourOne);
                g.fillEllipse(x1 - 8, y1 - 8, 16, 16);
                g.setColour(colourTwo);
                g.drawEllipse(x1 - 8, y1 - 8, 16, 16, 4);

            }

            if (!dynamic_cast<Junction*>(destination))
            {

                g.setColour(colourOne);
                g.fillEllipse(x2 - 8, y2 - 8, 16, 16);
                g.setColour(colourTwo);
                g.drawEllipse(x2 - 8, y2 - 8, 16, 16, 4);

            }

            if (!inverseIO)
            {
                p.startNewSubPath(x1, y1);
                p.cubicTo(x2, y1, x1, y2, x2, y2);
            }

        }

    }

    g.setColour(HackAudio::Colours::Gray);

    g.strokePath(p, juce::PathStrokeType(4));

}

void HackAudio::Diagram::resized()
{

}
