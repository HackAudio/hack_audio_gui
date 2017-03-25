#include "hack_audio_Viewport.h"

HackAudio::Viewport::Viewport()
{
    setInterceptsMouseClicks(true, false);
    contentContainer.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(contentContainer);

    resizeGuard = false;
}

HackAudio::Viewport::~Viewport()
{
    
}

void HackAudio::Viewport::setViewedComponent(juce::Component *c)
{
    contentContainer.removeAllChildren();
    contentContainer.addAndMakeVisible(c);
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

    if (e.getNumberOfClicks() > 1)
    {

        juce::Component* c = contentContainer.getChildComponent(0);
        c->centreWithSize(c->getWidth(), c->getHeight());

    }

    repaint();
}

void HackAudio::Viewport::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w)
{

    juce::Point<int> pos(contentContainer.getChildComponent(0)->getPosition());
    pos.x -= w.deltaX * 128;
    pos.y -= w.deltaY * 128;
    contentContainer.getChildComponent(0)->setTopLeftPosition(pos);

    repaint();
    
}

void HackAudio::Viewport::paint(juce::Graphics& g)
{

    g.setColour(HackAudio::Colours::Black);
    g.fillRect(contentContainer.getBounds());

}

void HackAudio::Viewport::paintOverChildren(juce::Graphics& g)
{

    juce::Component* c = contentContainer.getChildComponent(0);

    int x1 = contentContainer.getX();
    int y1 = contentContainer.getY() + contentContainer.getHeight() / 2;

    int x2 = c->getX();
    int y2 = c->getY() + c->getHeight() / 2;

    int x3 = c->getRight();
    int y3 = c->getY() + c->getHeight() / 2;

    int x4 = contentContainer.getX() + contentContainer.getWidth();
    int y4 = contentContainer.getY() + getHeight() / 2;

    g.setColour(HackAudio::Colours::Gray);
    g.fillEllipse(x2, y2 - 8, 16, 16);
    g.setColour(HackAudio::Colours::Black);
    g.drawEllipse(x2, y2 - 8, 16, 16, 4);

    g.setColour(HackAudio::Colours::Gray);
    g.fillEllipse(x3, y3 - 8, 16, 16);
    g.setColour(HackAudio::Colours::Black);
    g.drawEllipse(x3, y3 - 8, 16, 16, 4);

    juce::Path p;
    p.startNewSubPath(x1, y1);
    p.cubicTo(x2, y1, x1, y2, x2 + 4, y2);
    p.startNewSubPath(x3 + 4, y3);
    p.cubicTo(x4, y3, x3, y4, x4, y4);

    g.setColour(HackAudio::Colours::Gray);
    g.strokePath(p, juce::PathStrokeType(4));

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

}

void HackAudio::Viewport::resized()
{

    if (resizeGuard) { return; }

    int width  = getWidth();
    int height = getHeight();

    resizeGuard = true;
    setBounds(getX() - 16, getY(), width + 16, height);
    resizeGuard = false;

    contentContainer.centreWithSize(width, height);

    if(juce::Component* c = contentContainer.getChildComponent(0))
    {
        c->centreWithSize(c->getWidth(), c->getHeight());
    }

}
