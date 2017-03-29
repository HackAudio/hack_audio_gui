#include "hack_audio_Viewport.h"

HackAudio::Viewport::Viewport()
{
    setInterceptsMouseClicks(true, false);
    contentContainer.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(contentContainer);

    diagramName.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    diagramName.setJustificationType(juce::Justification::centred);

    juce::DropShadow shadow;
    shadow.radius = 4;
    shadow.offset.setXY(0, 2);
    dropShadow.setShadowProperties(shadow);
    diagramName.setComponentEffect(&dropShadow);

    addAndMakeVisible(diagramName);

    currentContent = nullptr;

    resizeGuard = false;
}

HackAudio::Viewport::~Viewport()
{
    
}

void HackAudio::Viewport::setViewedComponent(HackAudio::Diagram* d)
{

    if (contentContainer.getNumChildComponents() > 0)
    {
        currentContent->removeComponentListener(this);
        contentContainer.removeAllChildren();
    }

    currentContent = d;

    currentContent->addComponentListener(this);
    contentContainer.addAndMakeVisible(currentContent);

    diagramName.setText(currentContent->getName(), juce::dontSendNotification);

    resized();
    repaint();

}

void HackAudio::Viewport::mouseDown(const juce::MouseEvent& e)
{
    componentDragger.startDraggingComponent(contentContainer.getChildComponent(0), e);
}

void HackAudio::Viewport::mouseDrag(const juce::MouseEvent& e)
{
    componentDragger.dragComponent(contentContainer.getChildComponent(0), e, nullptr);
    repaint();
}

void HackAudio::Viewport::mouseUp(const juce::MouseEvent& e)
{

    if (e.getNumberOfClicks() > 1 && !componentAnimator.isAnimating())
    {

        juce::Rectangle<int> finalBounds = currentContent->getBounds();

        finalBounds.setCentre(contentContainer.getWidth()/2, contentContainer.getHeight()/2);

        componentAnimator.animateComponent(currentContent, finalBounds, 1.0, 250, false, 0, 0);

    }

    repaint();
}

void HackAudio::Viewport::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w)
{

    juce::Point<int> pos(currentContent->getPosition());
    pos.x -= w.deltaX * 128;
    pos.y -= w.deltaY * 128;
    currentContent->setTopLeftPosition(pos);

    repaint();
    
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

    juce::Array<juce::Component*> contentInputs = currentContent->getInputs();
    juce::Array<juce::Component*> contentOutputs = currentContent->getOutputs();

    juce::Path p;

    for (int i = 0; i < contentInputs.size(); ++i)
    {

        juce::Component* contentInput = contentInputs[i];

        if (contentInput->isVisible())
        {

            juce::Rectangle<int> contentInputBounds = contentInput->getScreenBounds().translated(-contentContainer.getScreenX(), -contentContainer.getScreenY());

            int x1 = contentContainer.getX();
            int y1 = contentContainer.getY() + contentContainer.getHeight() / 2;

            int x2 = contentInputBounds.getX();
            int y2 = contentInputBounds.getY() + contentInputBounds.getHeight() / 2;

            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentInput))
            {
                g.setColour(HackAudio::Colours::Gray);
                g.fillEllipse(x2, y2 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Black);
                g.drawEllipse(x2, y2 - 8, 16, 16, 4);
            }

            p.startNewSubPath(x1, y1);
            p.cubicTo(x2, y1, x1, y2, x2 + 4, y2);

        }

    }

    for (int i = 0; i < contentOutputs.size(); ++i)
    {

        juce::Component* contentOutput = contentOutputs[i];

        if (contentOutput->isVisible())
        {

            juce::Rectangle<int> contentOutputBounds = contentOutput->getScreenBounds().translated(-contentContainer.getScreenX(), -contentContainer.getScreenY());

            int x3 = contentOutputBounds.getRight();
            int y3 = contentOutputBounds.getY() + contentOutputBounds.getHeight() / 2;

            int x4 = contentContainer.getX() + contentContainer.getWidth();
            int y4 = contentContainer.getY() + getHeight() / 2;


            if (!dynamic_cast<HackAudio::Diagram::Junction*>(contentOutput))
            {
                g.setColour(HackAudio::Colours::Gray);
                g.fillEllipse(x3, y3 - 8, 16, 16);
                g.setColour(HackAudio::Colours::Black);
                g.drawEllipse(x3, y3 - 8, 16, 16, 4);
            }

            p.startNewSubPath(x3 + 4, y3);
            p.cubicTo(x4, y3, x3, y4, x4, y4);

        }

    }

    g.setColour(HackAudio::Colours::Gray);
    g.strokePath(p, juce::PathStrokeType(4));


    g.setGradientFill(
        juce::ColourGradient
        (
            HackAudio::Colours::Black.withAlpha(0.75f),
            0, 0,
            HackAudio::Colours::Black.withAlpha(0.0f),
            0, 4,
            false
        )
    );
    g.fillRect(0, 0, getWidth(), 4);

    g.setGradientFill(
        juce::ColourGradient
        (
            HackAudio::Colours::Black.withAlpha(0.0f),
            0, getHeight() - 4,
            HackAudio::Colours::Black.withAlpha(0.75f),
            0, getHeight(),
            false
        )
    );
    g.fillRect(0, getHeight() - 4, getWidth(), 4);

    g.setGradientFill(
        juce::ColourGradient
        (
            HackAudio::Colours::Black.withAlpha(0.75f),
            8, 0,
            HackAudio::Colours::Black.withAlpha(0.0f),
            12, 0,
            false
        )
    );
    g.fillRect(8, 0, 4, getHeight());

    g.setGradientFill(
        juce::ColourGradient
        (
            HackAudio::Colours::Black.withAlpha(0.0f),
            getWidth() - 12, 0,
            HackAudio::Colours::Black.withAlpha(0.75f),
            getWidth() - 8, 0,
            false
        )
    );
    g.fillRect(getWidth() - 12, 0, 4, getHeight());

    g.setColour(HackAudio::Colours::Gray);
    g.fillRect(0, 0, 8, getHeight());
    g.fillRect(getWidth() - 8, 0, 8, getHeight());

    g.setColour(HackAudio::Colours::Black);
    g.fillEllipse(0, (getHeight() / 2) - 8, 16, 16);
    g.setColour(HackAudio::Colours::Gray);
    g.drawEllipse(0, (getHeight() / 2) - 8, 16, 16, 4);

    g.setColour(HackAudio::Colours::Black);
    g.fillEllipse(getWidth() - 16, (getHeight() / 2) - 8, 16, 16);
    g.setColour(HackAudio::Colours::Gray);
    g.drawEllipse(getWidth() - 16, (getHeight() / 2) - 8, 16, 16, 4);

    repaint();

}

void HackAudio::Viewport::resized()
{

    if (resizeGuard) { return; }

    int width  = getWidth();
    int height = getHeight();

    resizeGuard = true;
    setBounds(getX() - 16, getY(), width + 16, height);
    resizeGuard = false;

    diagramName.setBounds(0, 0, width, 32);

    contentContainer.centreWithSize(width, height);

    if (currentContent)
    {
        currentContent->centreWithSize(currentContent->getWidth(), currentContent->getHeight());
    }

}
