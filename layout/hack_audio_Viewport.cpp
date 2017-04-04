#include "hack_audio_Viewport.h"

HackAudio::Viewport::Viewport()
{

    setInterceptsMouseClicks(true, false);
    setRepaintsOnMouseActivity(false);

    contentContainer.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(contentContainer);

    backButton.addListener(this);
    topButton.addListener(this);

    addAndMakeVisible(backButton);
    addAndMakeVisible(topButton);

    currentContent = nullptr;

    resizeGuard = false;

}

HackAudio::Viewport::~Viewport()
{
    
}

void HackAudio::Viewport::setDiagram(HackAudio::Diagram& d)
{

    backButton.setVisible(false);
    topButton.setVisible(false);

    if (contentContainer.getNumChildComponents() > 0)
    {
        currentContent->removeComponentListener(this);
        contentContainer.removeAllChildren();
    }

    currentContent = &d;

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());

    repaint();

}

void HackAudio::Viewport::traverseDown(HackAudio::Diagram *d)
{

    assert(contentContainer.getNumChildComponents() > 0);

    backButton.setVisible(true);
    topButton.setVisible(true);

    parentContent.add(currentContent);

    currentContent->removeComponentListener(this);

    contentContainer.removeAllChildren();

    currentContent = d;

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());

    repaint();

}

void HackAudio::Viewport::traverseUp()
{

    assert(contentContainer.getNumChildComponents() > 0);
    assert(parentContent.size() > 0);

    currentContent->removeComponentListener(this);

    contentContainer.removeAllChildren();
    
    currentContent = parentContent.removeAndReturn(parentContent.size() - 1);

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

void HackAudio::Viewport::traverseTop()
{

    assert(contentContainer.getNumChildComponents() > 0);
    assert(parentContent.size() > 0);

    backButton.setVisible(false);
    topButton.setVisible(false);

    currentContent->removeComponentListener(this);

    contentContainer.removeAllChildren();

    currentContent = parentContent.removeAndReturn(0);
    parentContent.clear();

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());
    
    repaint();

}

void HackAudio::Viewport::mouseMove(const juce::MouseEvent& e)
{

    if (!currentContent) { return; }

    for(juce::HashMap<juce::Component*, HackAudio::Diagram*>::Iterator it (currentContent->submap); it.next();)
    {
        if (it.getKey()->getScreenBounds().contains(e.getScreenPosition()))
        {
            it.getKey()->setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Gray.withMultipliedBrightness(1.25f));
        }
        else
        {
            it.getKey()->setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Gray);
        }
    }

}

void HackAudio::Viewport::mouseDown(const juce::MouseEvent& e)
{

    if (!currentContent) { return; }

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

            if (it.getKey()->isVisible() && it.getKey()->getScreenBounds().contains(e.getScreenPosition()))
            {
                it.getKey()->setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Gray);
                traverseDown(it.getValue());
                return;
            }
        }

        juce::Rectangle<int> finalBounds = currentContent->getBounds();

        finalBounds.setCentre(contentContainer.getWidth()/2, contentContainer.getHeight()/2);

        componentAnimator.animateComponent(currentContent, finalBounds, 1.0, 250, false, 0, 0);

        repaint();

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

    repaint();

}

void HackAudio::Viewport::paint(juce::Graphics& g)
{

    g.setColour(HackAudio::Colours::Black);
    g.fillRect(contentContainer.getBounds());

}

void HackAudio::Viewport::paintOverChildren(juce::Graphics& g)
{

    if (!currentContent) { return; }

    juce::Array<juce::Component*> contentInputs = currentContent->getDiagramInputs();
    juce::Array<juce::Component*> contentOutputs = currentContent->getDiagramOutputs();

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

            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentInput))
            {
                g.setColour(HackAudio::Colours::Gray);
                g.fillEllipse(x2 - 8, y2 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Black);
                g.drawEllipse(x2 - 8, y2 - 8, 16, 16, 4);
            }

            p.startNewSubPath(x1, y1);
            p.cubicTo(x2, y1, x1, y2, x2 + 4, y2);

            g.setColour(HackAudio::Colours::Gray);
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
            int y4 = contentContainer.getY() + getHeight() / 2;

            int offset = (dynamic_cast<HackAudio::Diagram::Junction*>(contentOutput)) ? 0 : 6;

            outputConnections.startNewSubPath(x3 + offset, y3);
            outputConnections.cubicTo(x4, y3, x3, y4, x4, y4);

            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentOutput))
            {
                g.setColour(HackAudio::Colours::Black);
                g.fillEllipse(x3 - 8, y3 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Gray);
                g.drawEllipse(x3 - 8, y3 - 8, 16, 16, 4);
            }
            
        }
        
    }



    // Viewport Shadows (Top, Bottom, Left, Right)
    // =========================================================================================
    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.75f),
       contentContainer.getX(), contentContainer.getY(),
       HackAudio::Colours::Black.withAlpha(0.0f),
       contentContainer.getX(), contentContainer.getY() + 4,
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getY(), contentContainer.getWidth(), 4);

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.0f),
       contentContainer.getX(), contentContainer.getBottom() - 4,
       HackAudio::Colours::Black.withAlpha(0.75f),
       contentContainer.getX(), contentContainer.getBottom(),
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getBottom() - 4, contentContainer.getWidth(), 4);

    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.75f),
       contentContainer.getX(), contentContainer.getY(),
       HackAudio::Colours::Black.withAlpha(0.0f),
       contentContainer.getX() + 4, contentContainer.getY(),
       false
      )
    );

    g.fillRect(contentContainer.getX(), contentContainer.getY(), 4, contentContainer.getHeight());
    
    g.setGradientFill(
      juce::ColourGradient
      (
       HackAudio::Colours::Black.withAlpha(0.0f),
       contentContainer.getRight() - 4, contentContainer.getY(),
       HackAudio::Colours::Black.withAlpha(0.75f),
       contentContainer.getRight(), contentContainer.getY(),
       false
      )
    );
    
    g.fillRect(contentContainer.getRight() - 4, contentContainer.getY(), 4, contentContainer.getHeight());



    // Output Connector
    // =========================================================================================
    g.setColour(HackAudio::Colours::Gray);
    g.fillRect(getWidth() - contentContainer.getX(), 0, contentContainer.getX(), getHeight());

    g.setColour(HackAudio::Colours::Gray);
    g.fillEllipse(contentContainer.getRight() - 8, (getHeight() / 2) - 8, 16, 16);
    g.setColour(HackAudio::Colours::Black);
    g.drawEllipse(contentContainer.getRight() - 8, (getHeight() / 2) - 8, 16, 16, 4);

    g.setColour(HackAudio::Colours::Gray);
    g.strokePath(outputConnections, juce::PathStrokeType(4));



    // Input Connector
    // =========================================================================================
    g.setColour(HackAudio::Colours::Gray);
    g.fillRect(0, 0, contentContainer.getX(), getHeight());

    g.setColour(HackAudio::Colours::Black);
    g.fillEllipse(contentContainer.getX() - 8, (getHeight() / 2) - 8, 16, 16);
    g.setColour(HackAudio::Colours::Gray);
    g.drawEllipse(contentContainer.getX() - 8, (getHeight() / 2) - 8, 16, 16, 4);



    // Navigation Buttons
    // =========================================================================================
    if (backButton.isVisible())
    {

        int width  = backButton.getWidth();
        int height = backButton.getHeight();

        int x = backButton.getX();
        int y = backButton.getY();

        juce::Button::ButtonState state = backButton.getState();

        if (state == juce::Button::ButtonState::buttonOver && state != juce::Button::ButtonState::buttonDown)
        {
            g.setColour(HackAudio::Colours::Gray);
        }
        else if (state == juce::Button::ButtonState::buttonOver && state == juce::Button::ButtonState::buttonDown)
        {
            g.setColour(HackAudio::Colours::Black);
        }
        else
        {
            g.setColour(HackAudio::Colours::White);
        }

        g.drawLine(x + (width/4), y + (height/2), x + (width - width/4), y + (height/16), 2);
        g.drawLine(x + (width/4), y + (height/2), x + (width - width/4), y + (height - height/16), 2);

    }

    if (topButton.isVisible())
    {

        int width  = topButton.getWidth();
        int height = topButton.getHeight();

        int x = topButton.getX();
        int y = topButton.getY();

        juce::Button::ButtonState state = topButton.getState();

        if (state == juce::Button::ButtonState::buttonOver && state != juce::Button::ButtonState::buttonDown)
        {
            g.setColour(HackAudio::Colours::Gray);
        }
        else if (state == juce::Button::ButtonState::buttonOver && state == juce::Button::ButtonState::buttonDown)
        {
            g.setColour(HackAudio::Colours::Black);
        }
        else
        {
            g.setColour(HackAudio::Colours::White);
        }

        g.drawLine(x + (width/16), y + (height - height/4), x + (width/2), y + (height/4), 2);
        g.drawLine(x + (width/2), y + (height/4), x + (width - width/16), y + (height - height/4), 2);
    }



    // Navigation Title
    // =========================================================================================
    g.setColour(HackAudio::Colours::Black.withAlpha(0.75f));
    g.drawFittedText(currentContent->getName(), 0, 2, getWidth(), 32, juce::Justification::centred, 1);
    g.setColour(HackAudio::Colours::White);
    g.drawFittedText(currentContent->getName(), 0, 0, getWidth(), 32, juce::Justification::centred, 1);

}

void HackAudio::Viewport::resized()
{

    if (resizeGuard) { return; }

    int width  = getWidth();
    int height = getHeight();

    resizeGuard = true;
    setBounds(getX() - 24, getY(), width + 24, height);
    resizeGuard = false;

    contentContainer.centreWithSize(width - 24, height);

    backButton.setBounds(contentContainer.getX() + 12, 8, 16, 16);
    topButton.setBounds(contentContainer.getRight() - 32, 8, 16, 16);

}
