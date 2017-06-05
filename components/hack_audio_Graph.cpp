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

HackAudio::Graph::Node::Node(HackAudio::Graph* graph) : owner(*graph)
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);
    setBroughtToFrontOnMouseClick(true);

    setSize(owner.nodeSize, owner.nodeSize);

}

void HackAudio::Graph::Node::setAxisLocking(bool shouldLockVertical, bool shouldLockHorizontal)
{

    assert(shouldLockVertical == shouldLockHorizontal == true);

    axisLockedY = shouldLockVertical;
    axisLockedX = shouldLockHorizontal;

}

void HackAudio::Graph::Node::setXValue(float newX)
{

    assert(newX >= 0.0f && newX <= 1.0f);

    setCentrePosition((getParentWidth() - getWidth()) * newX, getY() + getHeight() / 2);

}

float HackAudio::Graph::Node::getXValue() const
{

    float x = getX() + getWidth() / 2;
    return x / (getParentWidth() - getWidth() / 2);

}

void HackAudio::Graph::Node::setYValue(float newY)
{

    assert(newY >= 0.0f && newY <= 1.0f);

    setCentrePosition(getX() + getWidth() / 2, (getParentHeight() - getHeight()) * newY);

}

float HackAudio::Graph::Node::getYValue() const
{

    float y = getY() + getHeight() / 2;
    return y / (getParentHeight() - getHeight() / 2);

}

void HackAudio::Graph::Node::setZValue(float newZ)
{

    assert(newZ >= 0.0f && newZ <= 1.0f);

    z = newZ;

    owner.nodeChanged(owner.graphNodes.indexOf(this), getComponentID());

}

float HackAudio::Graph::Node::getZValue() const
{

    return z;

}

void HackAudio::Graph::Node::mouseEnter(const juce::MouseEvent& e)
{

    setMouseCursor(juce::MouseCursor::PointingHandCursor);

}

void HackAudio::Graph::Node::mouseExit(const juce::MouseEvent& e)
{

    setMouseCursor(juce::MouseCursor::NormalCursor);

}

void HackAudio::Graph::Node::mouseDown(const juce::MouseEvent& e)
{

    owner.dragger.startDraggingComponent(this, e);

}

void HackAudio::Graph::Node::mouseDrag(const juce::MouseEvent& e)
{

    int x = getX();
    int y = getY();

    owner.dragger.dragComponent(this, e, &owner.constraints);

    if (axisLockedX)
    {

        setTopLeftPosition(x, getY());

    }

    if (axisLockedY)
    {

        setTopLeftPosition(getX(), y);

    }

}

bool HackAudio::Graph::Node::keyPressed(const juce::KeyPress& key)
{

    int x = getX();
    int y = getY();

    int offset = (key.getModifiers().isShiftDown()) ? 30 : 10;

    if (key.getKeyCode() == key.upKey)
    {

        setTopLeftPosition(x, y - offset);
        owner.constraints.checkComponentBounds(this);
        return true;

    }
    else if (key.getKeyCode() == key.downKey)
    {

        setTopLeftPosition(x, y + offset);
        owner.constraints.checkComponentBounds(this);
        return true;

    }
    else if (key.getKeyCode() == key.leftKey)
    {

        setTopLeftPosition(x - offset, y);
        owner.constraints.checkComponentBounds(this);
        return true;

    }
    else if (key.getKeyCode() == key.rightKey)
    {

        setTopLeftPosition(x + offset, y);
        owner.constraints.checkComponentBounds(this);
        return true;

    }

    return false;

}

void HackAudio::Graph::Node::focusGained(juce::Component::FocusChangeType cause)
{

    repaint();

}

void HackAudio::Graph::Node::focusLost(juce::Component::FocusChangeType cause)
{

    repaint();

}

void HackAudio::Graph::Node::colourChanged()
{

    owner.colourChanged();

}

void HackAudio::Graph::Node::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillEllipse(0, 0, width, height);

    if (hasKeyboardFocus(true))
    {

        g.setColour(findColour(HackAudio::highlightColourId));

    }
    else
    {

        g.setColour(findColour(HackAudio::foregroundColourId));

    }

    g.drawEllipse(2, 2, width - 4, height - 4, 4);

}

HackAudio::Graph::Graph()
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setWantsKeyboardFocus(false);
    setMouseClickGrabsKeyboardFocus(false);

    addAndMakeVisible(contentContainer);

    nodeSize = 24;

    constraints.setSizeLimits(nodeSize, nodeSize, nodeSize, nodeSize);
    constraints.setMinimumOnscreenAmounts(nodeSize, nodeSize, nodeSize, nodeSize);

    startPoint = 0.5f;
    endPoint   = 0.5f;
    startAndEndShown = true;

}

HackAudio::Graph::~Graph()
{

    graphNodes.clear();

}

void HackAudio::Graph::add()
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->addComponentListener(this);
    graphNodes.add(n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

}

void HackAudio::Graph::add(const juce::String& nodeId)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->setComponentID(nodeId);
    n->addComponentListener(this);
    graphNodes.add(n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

}

void HackAudio::Graph::insert(int index)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->addComponentListener(this);
    graphNodes.insert(index, n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

}

void HackAudio::Graph::insert(int index, const juce::String& nodeId)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->setComponentID(nodeId);
    n->addComponentListener(this);
    graphNodes.insert(index, n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

}

HackAudio::Graph::Node* HackAudio::Graph::getNode(int index) const
{

    return graphNodes[index];

}

HackAudio::Graph::Node* HackAudio::Graph::getNode(const juce::String& nodeId) const
{

    return (HackAudio::Graph::Node*)contentContainer.findChildWithID(nodeId);

}

void HackAudio::Graph::remove(int index)
{

    graphNodes[index]->removeComponentListener(this);
    graphNodes.remove(index);
    nodeOrderChanged();

}

void HackAudio::Graph::remove(const juce::String& nodeId)
{

    HackAudio::Graph::Node* n = (HackAudio::Graph::Node*)contentContainer.findChildWithID(nodeId);

    n->removeComponentListener(this);
    graphNodes.removeObject(n);
    nodeOrderChanged();

}

void HackAudio::Graph::setNodeSize(int newSize)
{

    nodeSize = newSize;

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        HackAudio::Graph::Node* n = graphNodes[i];
        n->setSize(nodeSize, nodeSize);

    }

}

int HackAudio::Graph::getNodeSize() const
{

    return nodeSize;

}

void HackAudio::Graph::setGraphPointStatus(bool shouldShowStartAndEndPoints)
{

    startAndEndShown = shouldShowStartAndEndPoints;
    repaint();

}

bool HackAudio::Graph::getGraphPointStatus() const
{

    return startAndEndShown;

}

void HackAudio::Graph::setStartPoint(float pos)
{

    assert(pos >= 0.0f && pos <= 1.0f);
    startPoint = contentContainer.getY() + (pos * contentContainer.getHeight());
    repaint();

}

float HackAudio::Graph::getStartPoint() const
{

    return startPoint;

}

void HackAudio::Graph::setEndPoint(float pos)
{

    assert(pos >= 0.0f && pos <= 1.0f);
    endPoint = contentContainer.getY() + (pos * contentContainer.getHeight());
    repaint();

}

float HackAudio::Graph::getEndPoint() const
{

    return endPoint;

}

void HackAudio::Graph::setColourStatus(bool shouldSyncNodeColours)
{

    coloursSynced = shouldSyncNodeColours;
    colourChanged();

}

void HackAudio::Graph::addListener(HackAudio::Graph::Listener* listener)
{

    if (!listeners.contains(listener))
    {

        listeners.add(listener);

    }

}

void HackAudio::Graph::removeListener(HackAudio::Graph::Listener* listener)
{

    listeners.remove(listener);

}

void HackAudio::Graph::nodeOrderChanged()
{

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        HackAudio::Graph::Node* n = graphNodes[i];
        n->setExplicitFocusOrder(i + 1);
        colourChanged();

    }

}

void HackAudio::Graph::nodeChanged(int index, const juce::String& nodeId)
{

    listeners.call(&HackAudio::Graph::Listener::graphNodeChanged, this, index, nodeId);

}

void HackAudio::Graph::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{

    assert(!wasResized);    /* Warning: Graph::Nodes should never resize */

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        juce::Component* c = dynamic_cast<juce::Component*>(graphNodes[i]);

        if (c == &component)
        {

            constraints.checkComponentBounds(c);
            nodeChanged(i, c->getComponentID());
            repaint();
            return;

        }

    }

}

void HackAudio::Graph::enablementChanged()
{

    if (isEnabled())
    {

        setInterceptsMouseClicks(true, true);

    }
    else
    {

        setInterceptsMouseClicks(false, false);

    }

}

void HackAudio::Graph::colourChanged()
{

    if (coloursSynced)
    {

        for (int i = 0; i < graphNodes.size(); ++i)
        {

            HackAudio::Graph::Node* n = graphNodes[i];

            n->setColour(HackAudio::backgroundColourId, findColour(HackAudio::backgroundColourId));
            n->setColour(HackAudio::midgroundColourId, findColour(HackAudio::midgroundColourId));
            n->setColour(HackAudio::foregroundColourId, findColour(HackAudio::foregroundColourId));
            n->setColour(HackAudio::highlightColourId, findColour(HackAudio::highlightColourId));

        }

    }

}

void HackAudio::Graph::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(p);

    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillRect(contentContainer.getBounds());

    g.setColour(findColour(HackAudio::foregroundColourId));

    if (startAndEndShown)
    {

        p.clear();
        p.startNewSubPath(contentContainer.getX(), contentContainer.getY() + (contentContainer.getHeight() * startPoint));

        HackAudio::Graph::Node* n = graphNodes.getFirst();
        float x = (n->getX() + n->getWidth()  / 2) + contentContainer.getX();
        float y = (n->getY() + n->getHeight() / 2) + contentContainer.getY();

        p.lineTo(x, y);
        g.strokePath(p, juce::PathStrokeType(4, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    }

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        HackAudio::Graph::Node* n0 = graphNodes[i];
        HackAudio::Graph::Node* n1 = graphNodes[i + 1];

        float n0x = (n0->getX() + n0->getWidth() / 2) + contentContainer.getX();
        float n0y = (n0->getY() + n0->getHeight() / 2) + contentContainer.getY();

        juce::Point<float> start(n0x, n0y);
        juce::Point<float> end;

        if (n1)
        {

            float n1x = (n1->getX() + n1->getWidth() / 2) + contentContainer.getX();
            float n1y = (n1->getY() + n1->getHeight() / 2) + contentContainer.getY();
            end.setXY(n1x, n1y);

        }
        else
        {

            if (!startAndEndShown) { return; }

            float n1x = contentContainer.getRight();
            float n1y = contentContainer.getY() + (contentContainer.getHeight() * endPoint);
            end.setXY(n1x, n1y);

        }

        juce::Path interp;
        interp.startNewSubPath(start);
        interp.lineTo(end);
        g.strokePath(interp, juce::PathStrokeType(4, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    }

}

void HackAudio::Graph::paintOverChildren(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    int cX = contentContainer.getX();
    int cY = contentContainer.getY();
    int cW = contentContainer.getWidth();
    int cH = contentContainer.getHeight();

    juce::Path corners;

    const float csx = fmin(CORNER_RADIUS/2, width * 0.5f);
    const float csy = fmin(CORNER_RADIUS/2, height * 0.5f);
    const float cs45x = csx * 0.45f;
    const float cs45y = csy * 0.45f;

    corners.startNewSubPath(cX, cY + cH);
    corners.lineTo(cX + csx, cY + cH);
    corners.cubicTo(cX + cs45x, cY + cH, cX, cY + cH - cs45y, cX, cY + cH - csy);
    corners.closeSubPath();

    corners.startNewSubPath(cX + cW, cY);
    corners.lineTo(cX + cW - csx, cY);
    corners.cubicTo(cX + cW - cs45x, cY, cX + cW, cY + cs45y, cX + cW, cY + csy);
    corners.closeSubPath();

    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(corners);

    if (startAndEndShown)
    {

        const int size = nodeSize - (nodeSize / 6);

        juce::Rectangle<float> start = juce::Rectangle<float>(cX - (size/2), cY + (cH * startPoint) - (size/2), size, size);
        juce::Rectangle<float> end = juce::Rectangle<float>(cX + cW - (size/2), cY + (cH * endPoint) - (size/2), size, size);

        g.setColour(findColour(HackAudio::backgroundColourId));
        g.fillEllipse(start);
        g.fillEllipse(end);

        g.setColour(findColour(HackAudio::midgroundColourId));
        g.drawEllipse(start, 4);
        g.drawEllipse(end, 4);

    }

}

void HackAudio::Graph::resized()
{

    int width  = getWidth();
    int height = getHeight();

    contentContainer.centreWithSize(width - (CORNER_RADIUS * 2), height - (CORNER_RADIUS * 2));

}
