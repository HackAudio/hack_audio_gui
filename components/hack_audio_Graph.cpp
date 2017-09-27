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

    axisLockedX = false;
    axisLockedY = false;

    displayValues = true;

    z = 0.0f;

    setSize(owner.nodeSize, owner.nodeSize);

}

void HackAudio::Graph::Node::setAxisLocking(bool shouldLockVertical, bool shouldLockHorizontal)
{

    assert(!(shouldLockVertical == true && shouldLockHorizontal == true));

    axisLockedY = shouldLockVertical;
    axisLockedX = shouldLockHorizontal;

}

void HackAudio::Graph::Node::setValueDisplay(bool shouldDisplayValues)
{

    displayValues = shouldDisplayValues;

    if (!shouldDisplayValues)
    {

        setTooltip("");

    }

}

void HackAudio::Graph::Node::setXValue(float newX)
{

    assert(newX >= 0.0f && newX <= 1.0f);

    int width = getParentWidth() - getWidth();

    setCentrePosition((getWidth() / 2) + (width * newX), getY() + getHeight() / 2);

}

float HackAudio::Graph::Node::getXValue() const
{

    float x = getX() + getWidth() / 2;

    float minX = getWidth() / 2;
    float maxX = getParentWidth() - getWidth() / 2;

    return ((x - minX) / (maxX - minX));

}

void HackAudio::Graph::Node::setYValue(float newY)
{

    assert(newY >= 0.0f && newY <= 1.0f);

    int height = getParentHeight() - getHeight();

    setCentrePosition(getX() + getWidth() / 2, height - (height * newY) + (getHeight() / 2));

}

float HackAudio::Graph::Node::getYValue() const
{

    float y = getY() + getHeight() / 2;

    float maxY = getHeight() / 2;
    float minY = getParentHeight() - getHeight() / 2;

    return std::abs((y - minY) / (maxY - minY));

}

void HackAudio::Graph::Node::setZValue(float newZ)
{

    z = newZ;
    owner.nodeChanged(this);

}

float HackAudio::Graph::Node::getZValue() const
{

    return z;

}

juce::Point<int> HackAudio::Graph::Node::getNodePosition() const
{

    juce::Rectangle<int> parentBounds = getParentComponent()->getBounds();

    return juce::Point<int>(
        getX() + (getWidth() / 2) + parentBounds.getX(),
        getY() + (getHeight() / 2) + parentBounds.getY()
    );

}

void HackAudio::Graph::Node::updateTooltip()
{

    if (!displayValues)
        return;

    juce::String tooltipText, x, y;

    if (getComponentID().isNotEmpty())
        tooltipText += getComponentID() + "\n";

    if (owner.getXUnits().isNotEmpty())
    {

        juce::var minvar = owner.getXMin();
        juce::var maxvar = owner.getXMax();

        auto min = (minvar.isInt()) ? (int)minvar : (minvar.isDouble()) ? (double)minvar : 0.0f;
        auto max = (maxvar.isInt()) ? (int)maxvar : (maxvar.isDouble()) ? (double)maxvar : 0.0f;

        x = juce::String(
            ((max - min) * getXValue()) + min
        );

        x += owner.getXUnits();

    }
    else
    {

        x = juce::String(getXValue());

        if (x.length() > 4)
            x = x.dropLastCharacters(x.length() - 4);

    }

    if (owner.getYUnits().isNotEmpty())
    {

        juce::var minvar = owner.getYMin();
        juce::var maxvar = owner.getYMax();

        auto min = (minvar.isInt()) ? (int)minvar : (minvar.isDouble()) ? (double)minvar : 0.0f;
        auto max = (maxvar.isInt()) ? (int)maxvar : (maxvar.isDouble()) ? (double)maxvar : 0.0f;

        y = juce::String(
            ((max - min) * getYValue()) + min
        );
        
        y += owner.getYUnits();

    }
    else
    {

        y = juce::String(getYValue());

        if (y.length() > 4)
            y = y.dropLastCharacters(y.length() - 4);

    }

    tooltipText += x + ", " + y;

    setTooltip(tooltipText);

}

void HackAudio::Graph::Node::mouseEnter(const juce::MouseEvent& e)
{

    // Updating the tooltip on mouseEnter ensures it's always set when we need it
    updateTooltip();

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

    updateTooltip();

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

    g.drawEllipse(2, 2, width - 4, height - 4, owner.getLineWidth());

}

HackAudio::Graph::Graph()
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setWantsKeyboardFocus(false);
    setMouseClickGrabsKeyboardFocus(false);

    contentContainer.addMouseListener(this, false);
    addAndMakeVisible(contentContainer);

    nodeSize = 24;
    lineWidth = 4.0f;

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

HackAudio::Graph::Node* HackAudio::Graph::add()
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->addComponentListener(this);
    graphNodes.add(n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

    return n;

}

HackAudio::Graph::Node* HackAudio::Graph::add(const juce::String& nodeId)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->setComponentID(nodeId);
    n->addComponentListener(this);
    graphNodes.add(n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

    return n;

}

HackAudio::Graph::Node* HackAudio::Graph::insert(int index)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->addComponentListener(this);
    graphNodes.insert(index, n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

    return n;

}

HackAudio::Graph::Node* HackAudio::Graph::insert(int index, const juce::String& nodeId)
{

    HackAudio::Graph::Node* n = new HackAudio::Graph::Node(this);
    n->setComponentID(nodeId);
    n->addComponentListener(this);
    graphNodes.insert(index, n);

    contentContainer.addAndMakeVisible(n);
    n->setCentreRelative(0.5f, 0.5f);

    nodeOrderChanged();

    return n;

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

    assert(nodeSize > 0);

    nodeSize = newSize;

    constraints.setSizeLimits(nodeSize, nodeSize, nodeSize, nodeSize);

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        HackAudio::Graph::Node* n = graphNodes[i];

        float x = n->getXValue();
        float y = n->getYValue();

        n->setSize(nodeSize, nodeSize);

        n->setXValue(x);
        n->setYValue(y);

    }

}

int HackAudio::Graph::getNodeSize() const
{

    return nodeSize;

}

void HackAudio::Graph::setLineWidth(float width)
{

    assert(lineWidth > 0.0f);

    lineWidth = width;
    repaint();

}

float HackAudio::Graph::getLineWidth() const
{

    return lineWidth;

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

void HackAudio::Graph::setXRange(int min, int max)
{

    xmin = min;
    xmax = max;

}

void HackAudio::Graph::setXRange(float min, float max)
{

    xmin = min;
    xmax = max;

}

void HackAudio::Graph::setXRange(double min, double max)
{

    xmin = min;
    xmax = max;
    
}

juce::var HackAudio::Graph::getXMin() const
{

    return xmin;

}

juce::var HackAudio::Graph::getXMax() const
{

    return xmax;
    
}

void HackAudio::Graph::setXUnits(const juce::String& units)
{

    xUnits = units;

}

juce::String HackAudio::Graph::getXUnits() const
{

    return xUnits;

}

void HackAudio::Graph::setYRange(int min, int max)
{

    ymin = min;
    ymax = max;

}

void HackAudio::Graph::setYRange(float min, float max)
{

    ymin = min;
    ymax = max;

}

void HackAudio::Graph::setYRange(double min, double max)
{

    ymin = min;
    ymax = max;

}

juce::var HackAudio::Graph::getYMin() const
{

    return ymin;

}

juce::var HackAudio::Graph::getYMax() const
{

    return ymax;

}

void HackAudio::Graph::setYUnits(const juce::String& units)
{

    yUnits = units;

}

juce::String HackAudio::Graph::getYUnits() const
{
    
    return yUnits;
    
}

void HackAudio::Graph::setColourStatus(bool shouldSyncNodeColours)
{

    coloursSynced = shouldSyncNodeColours;
    colourChanged();

}

juce::Path HackAudio::Graph::drawGraph(juce::Rectangle<int> graphBounds, juce::Point<int> start, juce::Point<int> end)
{

    return juce::Path();

}

void HackAudio::Graph::drawGraphBackground(juce::Graphics &g, juce::Rectangle<int> graphBounds)
{

    return;

}

juce::Path HackAudio::Graph::drawLineBetween(HackAudio::Graph::Node* nodeOne, Graph::Node* nodeTwo)
{

    juce::Path p;

    juce::Point<int> n1 = nodeOne->getNodePosition();
    juce::Point<int> n2 = nodeTwo->getNodePosition();

    p.startNewSubPath(n1.getX(), n1.getY());
    p.lineTo(n2.getX(), n2.getY());

    return p;

}

juce::Path HackAudio::Graph::drawLineFromStart(juce::Point<int> graphStart, HackAudio::Graph::Node* firstNode)
{

    juce::Path p;

    juce::Point<int> n = firstNode->getNodePosition();

    p.startNewSubPath(graphStart.getX(), graphStart.getY());
    p.lineTo(n.getX(), n.getY());

    return p;

}

juce::Path HackAudio::Graph::drawLineToEnd(HackAudio::Graph::Node* lastNode, juce::Point<int> graphEnd)
{

    juce::Path p;

    juce::Point<int> n = lastNode->getNodePosition();

    p.startNewSubPath(n.getX(), n.getY());
    p.lineTo(graphEnd.getX(), graphEnd.getY());

    return p;
    
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

void HackAudio::Graph::nodeChanged(HackAudio::Graph::Node* n)
{

    listeners.call(&HackAudio::Graph::Listener::graphNodeChanged, this, n);

}

void HackAudio::Graph::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{

    if (!wasMoved)
        return;

    for (int i = 0; i < graphNodes.size(); ++i)
    {

        juce::Component* c = dynamic_cast<juce::Component*>(graphNodes[i]);

        if (c == &component)
        {

            constraints.checkComponentBounds(c);
            nodeChanged(graphNodes[i]);
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

void HackAudio::Graph::mouseUp(const juce::MouseEvent &e)
{

    unfocusAllComponents();

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

    drawGraphBackground(g, contentContainer.getBounds());

    g.setColour(findColour(HackAudio::foregroundColourId));

    if (graphNodes.size() > 0)
    {

        if (startAndEndShown)
        {

            juce::Point<int> graphStart = juce::Point<int>(
                contentContainer.getX(),
                startPoint
            );

            HackAudio::Graph::Node* n = graphNodes.getFirst();

            juce::Path interp = drawLineFromStart(graphStart, n);

            g.strokePath(interp, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        }

        for (int i = 0; i < graphNodes.size(); ++i)
        {

            HackAudio::Graph::Node* n0 = graphNodes[i];
            HackAudio::Graph::Node* n1 = graphNodes[i + 1];

            juce::Path interp;

            if (n1)
            {

                interp = drawLineBetween(n0, n1);

            }
            else
            {

                if (!startAndEndShown)
                    return;

                juce::Point<int> graphEnd = juce::Point<int>(
                    contentContainer.getRight(),
                    endPoint
                );

                interp = drawLineToEnd(n0, graphEnd);

            }

            g.strokePath(interp, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        }

    }
    else
    {

        juce::Point<int> graphStart = juce::Point<int>(
                contentContainer.getX(),
                startPoint
        );

        juce::Point<int> graphEnd = juce::Point<int>(
                    contentContainer.getRight(),
                    endPoint
        );

        juce::Path interp = drawGraph(contentContainer.getBounds(), graphStart, graphEnd);
        g.strokePath(interp, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

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

        juce::Rectangle<float> start = juce::Rectangle<float>(cX - (size/2), startPoint - (size/2), size, size);
        juce::Rectangle<float> end = juce::Rectangle<float>(cX + cW - (size/2), endPoint - (size/2), size, size);

        g.setColour(findColour(HackAudio::backgroundColourId));
        g.fillEllipse(start);
        g.fillEllipse(end);

        g.setColour(findColour(HackAudio::midgroundColourId));
        g.drawEllipse(start, lineWidth);
        g.drawEllipse(end, lineWidth);

    }

}

void HackAudio::Graph::resized()
{

    int width  = getWidth();
    int height = getHeight();

    contentContainer.centreWithSize(width - (CORNER_RADIUS * 2), height - (CORNER_RADIUS * 2));

}
