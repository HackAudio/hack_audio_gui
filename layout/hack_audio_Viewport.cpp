#include "hack_audio_Viewport.h"

HackAudio::Viewport::Viewport()
{
//    setScrollOnDragEnabled(true);
//    setScrollBarsShown(false, false, true, true);
    setInterceptsMouseClicks(true, false);
}

HackAudio::Viewport::~Viewport()
{
    
}

void HackAudio::Viewport::setViewedComponent(juce::Component *c)
{
    removeAllChildren();
    addAndMakeVisible(c);
}

void HackAudio::Viewport::mouseDown(const juce::MouseEvent& e)
{
    componentDragger.startDraggingComponent(getChildComponent(0), e);
}

void HackAudio::Viewport::mouseDrag(const juce::MouseEvent& e)
{
    componentDragger.dragComponent(getChildComponent(0), e, nullptr);
    repaint();
}

void HackAudio::Viewport::mouseUp(const juce::MouseEvent& e)
{

    if (e.getNumberOfClicks() > 1)
    {

        juce::Component* c = getChildComponent(0);
        c->centreWithSize(c->getWidth(), c->getHeight());

    }

    repaint();
}

void HackAudio::Viewport::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w)
{

    juce::Point<int> pos(getChildComponent(0)->getPosition());
    pos.x -= w.deltaX * 128;
    pos.y -= w.deltaY * 128;
    getChildComponent(0)->setTopLeftPosition(pos);
    
}

void HackAudio::Viewport::paint(juce::Graphics& g)
{
    g.fillAll(HackAudio::Colours::Black);
}

void HackAudio::Viewport::paintOverChildren(juce::Graphics& g)
{

    juce::Component* c = getChildComponent(0);

    int x1 = 0;
    int y1 = getHeight() / 2;

    int x2 = c->getX();
    int y2 = c->getY() + c->getHeight() / 2;

    int x3 = c->getRight();
    int y3 = c->getY() + c->getHeight() / 2;

    int x4 = getWidth();
    int y4 = getHeight() / 2;

    juce::Path p;
    p.startNewSubPath(x1, y1);
    p.cubicTo(x2, y1, x1, y2, x2, y2);
    p.startNewSubPath(x3, y3);
    p.cubicTo(x4, y3, x3, y4, x4, y4);

    g.setColour(HackAudio::Colours::Gray);
    g.strokePath(p, juce::PathStrokeType(4));

}
