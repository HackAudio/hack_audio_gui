HackAudio::Viewport::Viewport()
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setInterceptsMouseClicks(true, true);
    setRepaintsOnMouseActivity(false);

    contentContainer.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(contentContainer);


    backButton.buttonDirection = HackAudio::NavigationButton::Left;
    topButton.buttonDirection = HackAudio::NavigationButton::Up;

    backButton.addListener(this);
    topButton.addListener(this);

    addAndMakeVisible(backButton);
    addAndMakeVisible(topButton);

    currentContent = nullptr;

}

HackAudio::Viewport::~Viewport()
{
    
}

void HackAudio::Viewport::setDiagram(HackAudio::Diagram& d)
{

    if (currentContent)
    {

        currentContent->removeComponentListener(this);

        for (int child = 0; child < currentContent->getNumChildComponents(); ++child)
        {
            currentContent->getChildComponent(child)->removeMouseListener(this);
        }

    }

    contentContainer.removeAllChildren();

    currentContent = &d;

    for (int child = 0; child < currentContent->getNumChildComponents(); ++child)
    {
        currentContent->getChildComponent(child)->addMouseListener(this, false);
    }

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());

    parentContent.clear();
    backButton.setVisible(false);
    topButton.setVisible(false);

    repaint();

}

void HackAudio::Viewport::clearDiagram()
{

    if (currentContent)
    {

        currentContent->removeComponentListener(this);

        for (int child = 0; child < currentContent->getNumChildComponents(); ++child)
        {
            currentContent->getChildComponent(child)->removeMouseListener(this);
        }

    }

    contentContainer.removeAllChildren();

    currentContent = nullptr;

    parentContent.clear();
    backButton.setVisible(false);
    topButton.setVisible(false);

    repaint();

}

void HackAudio::Viewport::traverseDown(HackAudio::Diagram &d)
{

    assert(contentContainer.getNumChildComponents() > 0);   /* Warning: Viewport Is Empty */

    backButton.setVisible(true);
    topButton.setVisible(true);

    parentContent.add(currentContent);

    setDiagramViaTraversal(d);

    backButton.setVisible(true);
    topButton.setVisible(true);

}

void HackAudio::Viewport::traverseUp()
{

    assert(contentContainer.getNumChildComponents() > 0);   /* Warning: Viewport Is Empty */
    assert(parentContent.size() > 0);                       /* Warning: Viewport Is Already On Top-Level Diagram */
    
    HackAudio::Diagram* d = parentContent.removeAndReturn(parentContent.size() - 1);

    setDiagramViaTraversal(*d);

}

void HackAudio::Viewport::traverseTop()
{

    assert(contentContainer.getNumChildComponents() > 0);   /* Warning: Viewport Is Empty */
    assert(!parentContent.isEmpty());                       /* Warning: Viewport Is Already On Top-Level Diagram */

    setDiagram(*parentContent.getFirst());

}

bool HackAudio::Viewport::isInTraversal(HackAudio::Diagram& d) const
{

    return (parentContent.contains(&d) || currentContent == &d);

}

void HackAudio::Viewport::setDiagramViaTraversal(HackAudio::Diagram &d)
{

    if (currentContent)
    {
        currentContent->removeComponentListener(this);

        for (int child = 0; child < currentContent->getNumChildComponents(); ++child)
        {
            currentContent->getChildComponent(child)->removeMouseListener(this);
        }

    }

    contentContainer.removeAllChildren();

    currentContent = &d;

    for (int child = 0; child < currentContent->getNumChildComponents(); ++child)
    {
        currentContent->getChildComponent(child)->addMouseListener(this, false);
    }

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());

    if (parentContent.size() == 0)
    {
        backButton.setVisible(false);
        topButton.setVisible(false);
    }
    
    repaint();

}

void HackAudio::Viewport::mouseEnter(const juce::MouseEvent& e)
{

    if (e.eventComponent != this)
    {
        e.eventComponent->mouseEnter(e);
    }

}

void HackAudio::Viewport::mouseExit(const juce::MouseEvent& e)
{

    if (e.eventComponent != this)
    {
        e.eventComponent->mouseExit(e);
    }

}

void HackAudio::Viewport::mouseDown(const juce::MouseEvent& e)
{

    if (!currentContent) { return; }

    if (e.eventComponent != this)
    {

        e.eventComponent->mouseDown(e);

    }

    componentDragger.startDraggingComponent(contentContainer.getChildComponent(0), e);

}

void HackAudio::Viewport::mouseDrag(const juce::MouseEvent& e)
{

    if (!currentContent) { return; }

    componentDragger.dragComponent(contentContainer.getChildComponent(0), e, nullptr);

}

void HackAudio::Viewport::mouseUp(const juce::MouseEvent& e)
{

    if (!currentContent) { return; }

    if (e.getNumberOfClicks() > 1 && !componentAnimator.isAnimating())
    {

        for(juce::HashMap<juce::Component*, HackAudio::Diagram*>::Iterator it (currentContent->submap); it.next();)
        {

            if (it.getKey()->isVisible() && it.getKey() == e.eventComponent)
            {
                traverseDown(*it.getValue());
                return;
            }
        }

        juce::Rectangle<int> finalBounds = currentContent->getBounds();

        finalBounds.setCentre(contentContainer.getWidth()/2, contentContainer.getHeight()/2);

        componentAnimator.animateComponent(currentContent, finalBounds, 1.0, 250, false, 0, 0);

        repaint();

    }
    else if (!e.mouseWasDraggedSinceMouseDown())
    {

        if (e.eventComponent != this)
        {
            e.eventComponent->mouseUp(e);
        }

    }

}

void HackAudio::Viewport::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w)
{

    if (!currentContent) { return; }

    juce::Point<int> pos(currentContent->getPosition());

    pos.x -= w.deltaX * MOUSEWHEEL_SENSITIVITY;
    pos.y -= w.deltaY * MOUSEWHEEL_SENSITIVITY;
    currentContent->setTopLeftPosition(pos);

}

void HackAudio::Viewport::buttonClicked(juce::Button *b)
{

    if (b == &backButton)
    {
        traverseUp();
    }

    if (b == &topButton)
    {
        traverseTop();
    }

}

void HackAudio::Viewport::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{

    repaint(contentContainer.getBounds());

}

void HackAudio::Viewport::paint(juce::Graphics& g)
{

    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillRect(contentContainer.getBounds());

}

void HackAudio::Viewport::paintOverChildren(juce::Graphics& g)
{

    if (!currentContent) { return; }

    int width  = getWidth();
    int height = getHeight();

    juce::Array<juce::Component*> contentInputs = currentContent->getDiagramInputs();
    juce::Array<juce::Component*> contentOutputs = currentContent->getDiagramOutputs();

    int cX = contentContainer.getX();
    int cY = contentContainer.getY();
    int cW = contentContainer.getWidth();
    int cH = contentContainer.getHeight();

    // Input Nodes
    // =========================================================================================
    for (int i = 0; i < contentInputs.size(); ++i)
    {

        juce::Component* contentInput = contentInputs[i];

        if (contentInput->isVisible())
        {

            juce::Path p;

            juce::Rectangle<int> contentInputBounds = contentInput->getScreenBounds().translated(-getScreenX(), -getScreenY());

            int x1 = contentContainer.getX();
            int y1 = contentContainer.getY() + contentContainer.getHeight() / 2;

            int x2 = contentInputBounds.getX();
            int y2 = contentInputBounds.getY() + contentInputBounds.getHeight() / 2;

            if (x2 < x1) { continue; }

            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentInput))
            {
                g.setColour(findColour(HackAudio::midgroundColourId));
                g.fillEllipse(x2 - 8, y2 - 8, 16, 16);
                g.setColour(findColour(HackAudio::backgroundColourId));
                g.drawEllipse(x2 - 8, y2 - 8, 16, 16, 4);
            }


            p.startNewSubPath(x1, y1);

            if (x2 - x1 < 64)
            {

                p.cubicTo(x2, y1, x1, y2, x2 + 4, y2);

            }
            else
            {

                p.cubicTo(x1 + 64, y1, x1, y2, x1 + 64, y2);
                p.startNewSubPath(x1 + 64, y2);
                p.cubicTo(x1 + 64, y2, x2, y2, x2, y2);

            }

            g.setColour(findColour(HackAudio::midgroundColourId));
            g.strokePath(p, juce::PathStrokeType(4));

        }

    }



    // Output Nodes
    // =========================================================================================
    juce::Path outputConnections;

    for (int i = 0; i < contentOutputs.size(); ++i)
    {

        juce::Component* contentOutput = contentOutputs[i];

        if (contentOutput->isVisible())
        {

            juce::Path p;

            juce::Rectangle<int> contentOutputBounds = contentOutput->getScreenBounds().translated(-getScreenX(), -getScreenY());

            int x3 = contentOutputBounds.getRight();
            int y3 = contentOutputBounds.getY() + contentOutputBounds.getHeight() / 2;

            int x4 = contentContainer.getX() + contentContainer.getWidth();
            int y4 = getHeight() / 2;

            if (x3 > x4) { continue; }

            int offset = (dynamic_cast<HackAudio::Diagram::Junction*>(contentOutput)) ? 0 : 6;

            if (x4 - x3 < 64)
            {

                outputConnections.startNewSubPath(x3 + offset, y3);
                outputConnections.cubicTo(x4, y3, x3, y4, x4 + 4, y4);

            }
            else
            {

                outputConnections.startNewSubPath(x3 + offset, y3);
                outputConnections.lineTo(x4 - 64, y3);
                outputConnections.startNewSubPath(x4 - 64, y3);
                outputConnections.cubicTo(x4, y3, x4 - 64, y4, x4, y4);
                
            }

            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentOutput))
            {
                g.setColour(findColour(HackAudio::backgroundColourId));
                g.fillEllipse(x3 - 8, y3 - 8, 16, 16);
                g.setColour(findColour(HackAudio::midgroundColourId));
                g.drawEllipse(x3 - 8, y3 - 8, 16, 16, 4);
            }
            
        }
        
    }

    // Viewport Shadows (Top, Bottom, Left, Right, Input, Output)
    // =========================================================================================
    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.75f),
       cX, cY,
       HackAudio::Colours::Black.withAlpha(0.0f),
       cX, cY + 4,
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getY(), contentContainer.getWidth(), 4);

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.0f),
       cX, (cY + cH) - 4,
       HackAudio::Colours::Black.withAlpha(0.75f),
       cX, cY + cH,
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getBottom() - 4, contentContainer.getWidth(), 4);

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.75f),
       cX, cY,
       HackAudio::Colours::Black.withAlpha(0.0f),
       cX + 4, cY,
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getY(), 4, contentContainer.getHeight());
    
    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.0f),
       (cX + cW) - 4, cY,
       HackAudio::Colours::Black.withAlpha(0.75f),
       (cX + cW), cY,
       false
      )
    );
    
    g.fillRect(contentContainer.getRight() - 4, contentContainer.getY(), 4, contentContainer.getHeight());

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(1.0f),
       cX, height / 2,
       HackAudio::Colours::Black.withAlpha(0.0f),
       cX + 16, height / 2,
       true
      )
    );

    g.fillEllipse(contentContainer.getX() - 12, (getHeight() / 2) - 12, 24, 24);

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(1.0f),
       (cX + cW), height / 2,
       HackAudio::Colours::Black.withAlpha(0.0f),
       (cX + cW) - 16, height / 2,
       true
      )
    );

    g.fillEllipse((cX + cW) - 12, (height / 2) - 12, 24, 24);


    // Frame & Rounded Corners (CORNER_CONFIG)
    // =========================================================================================
    const float csx = fmin(CORNER_RADIUS/2, width * 0.5f);
    const float csy = fmin(CORNER_RADIUS/2, height * 0.5f);
    const float cs45x = csx * 0.45f;
    const float cs45y = csy * 0.45f;

    juce::Path corners;

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

    juce::Path p;

    p.startNewSubPath(0, 0);
    p.lineTo(width - csx, 0);
    p.cubicTo(width - cs45x, 0, width, cs45y, width, csy);
    p.lineTo(0, csy);
    p.closeSubPath();

    p.addRectangle(width - csx, csy, csx, height - csy);
    p.addRectangle(csx, height - csy, width - csx, csy);

    p.startNewSubPath(0, csy);
    p.lineTo(0, height - csy);
    p.cubicTo(0, height - cs45y, cs45x, height, csx, height);
    p.lineTo(csx, csy);
    p.closeSubPath();

    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(p);

    // Output Connector
    // =========================================================================================
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillEllipse((cX + cW) - 8, (height / 2) - 8, 16, 16);
    g.setColour(findColour(HackAudio::backgroundColourId));
    g.drawEllipse((cX + cW) - 8, (height / 2) - 8, 16, 16, 4);

    g.setColour(findColour(HackAudio::midgroundColourId));
    g.strokePath(outputConnections, juce::PathStrokeType(4));

    // Input Connector
    // =========================================================================================
    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillEllipse(cX - 8, (height / 2) - 8, 16, 16);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.drawEllipse(cX - 8, (height / 2) - 8, 16, 16, 4);

    // Navigation Title
    // =========================================================================================
    g.setFont(HackAudio::Fonts::NowRegular.withHeight(DEFAULT_FONTSIZE));
    g.setColour(findColour(HackAudio::backgroundColourId).withAlpha(0.75f));
    g.drawFittedText(currentContent->getName(), cX + (CORNER_RADIUS * 2), cY + 2, cW - (CORNER_RADIUS * 4), 32, juce::Justification::centred, 0, 1);
    g.setColour(findColour(HackAudio::foregroundColourId));
    g.drawFittedText(currentContent->getName(), cX + (CORNER_RADIUS * 2), cY, cW - (CORNER_RADIUS * 4), 32, juce::Justification::centred, 0, 1);

}

void HackAudio::Viewport::resized()
{

    int width  = getWidth();
    int height = getHeight();

    contentContainer.centreWithSize(width - 24, height - 24);

    backButton.setBounds(contentContainer.getX() + 12, contentContainer.getY() + 8, 16, 16);
    topButton.setBounds(contentContainer.getRight() - 32, contentContainer.getY() + 8, 16, 16);

}
