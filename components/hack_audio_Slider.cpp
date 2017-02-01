#include "hack_audio_Slider.h"

HackAudio::Slider::Slider()
{
    setSliderStyle(juce::Slider::LinearVertical);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setSliderSnapsToMousePosition(false);

    pipAreas.resize(10);

    animationAcc = 0;
    animationVel = 1;

    animationStart = animationEnd = juce::Point<int>(0, 0);

    isAnimating   = false;
    isResizing = false;

    setSize(128, 384);
}

HackAudio::Slider::~Slider()
{

}

void HackAudio::Slider::setDefaultValue(double defaultValue)
{
    setDoubleClickReturnValue(true, defaultValue);
}

void HackAudio::Slider::mouseDown(const juce::MouseEvent& e)
{
    if (sliderArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {
        juce::Slider::mouseDown(e);
    }

    for (int i = 0; i < pipAreas.size(); ++i)
    {
        if (pipAreas[i].contains(e.getPosition()))
        {
            animationStart = thumbArea.getPosition();

            if (getSliderStyle() == juce::Slider::LinearVertical)
            {
                animationEnd = juce::Point<int>(0, ((i / (float)pipAreas.size())) * (sliderArea.getHeight() - sliderArea.getWidth()));
            }
            else if (getSliderStyle() == juce::Slider::LinearHorizontal)
            {
                animationEnd = juce::Point<int>(((i / pipAreas.size()) / getMaximum()) * (sliderArea.getWidth() - sliderArea.getHeight()), 0);
            }

            setValue((i / (float)pipAreas.size()) * getMaximum());

//            startTimerHz(30);
        }
    }
}

void HackAudio::Slider::timerCallback()
{
    int width  = getWidth();
    int height = getHeight();

    if (getSliderStyle() == juce::Slider::LinearVertical)
    {
        if (thumbArea.getY() < (animationStart.getY() + animationEnd.getY()) / 2)
        {
            animationAcc += 0.1;
        }
        else
        {
            animationAcc -= 0.1;
        }

        animationVel += animationAcc;

        thumbArea.translate(0, animationVel);
    }
    else if (getSliderStyle() == juce::Slider::LinearHorizontal)
    {
        if (thumbArea.getX() < (animationStart.getX() + animationEnd.getX()) / 2)
        {
            animationAcc += 0.1;
        }
        else
        {
            animationAcc -= 0.1;
        }

        animationVel += animationAcc;

        thumbArea.translate(animationVel, 0);
    }

    if (thumbArea.getPosition().getDistanceFrom(animationEnd) < 5)
    {
        animationAcc = 0;
        animationVel = 1;
        thumbArea.setPosition(animationEnd);
        stopTimer();
        repaint();
    }

    repaint();
}

void HackAudio::Slider::paint(juce::Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    // Draw Background
    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, 32, 32, false, true, true, false);
    g.setColour(HackAudio::Colours::Black);
    g.fillPath(p);

    // Draw Slider Tracks and Thumb
    g.setColour(HackAudio::Colours::Gray);
    g.fillRoundedRectangle(sliderArea.getX(), sliderArea.getY(), sliderArea.getWidth(), sliderArea.getHeight(), 8);

    if (getSliderStyle() == juce::Slider::LinearVertical)
    {
        g.setColour(HackAudio::Colours::Cyan);
        g.fillRoundedRectangle(sliderArea.getX(), sliderArea.getBottom() - ((getValue()/getMaximum()) * sliderArea.getHeight()), sliderArea.getWidth(), (getValue()/getMaximum()) * sliderArea.getHeight(), 8);

        g.setColour(HackAudio::Colours::White);
        g.fillEllipse(thumbArea.getX(), sliderArea.getBottom() - ((getValue()/getMaximum()) * sliderArea.getHeight() + thumbArea.getHeight() / 2), thumbArea.getWidth(), thumbArea.getHeight());

        g.setColour(HackAudio::Colours::Black);
        g.drawEllipse(thumbArea.getX(), sliderArea.getBottom() - ((getValue()/getMaximum()) * sliderArea.getHeight() + thumbArea.getHeight() / 2), thumbArea.getWidth(), thumbArea.getHeight(), thumbArea.getWidth()/4);
    }
    else if (getSliderStyle() == juce::Slider::LinearHorizontal)
    {
        g.setColour(HackAudio::Colours::Cyan);
        g.fillRoundedRectangle(sliderArea.getX(), sliderArea.getY(), (getValue()/getMaximum()) * sliderArea.getWidth(), sliderArea.getHeight(), 8);

        g.setColour(HackAudio::Colours::White);
        g.fillEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

        g.setColour(HackAudio::Colours::Black);
        g.drawEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8);
    }

    // Draw Slider Pips
    for (int i = 0; i < pipAreas.size(); ++i)
    {
        if ((i / (float)pipAreas.size()) - (getValue() / getMaximum()) > 0.0025)
        {
            g.setColour(HackAudio::Colours::Gray);
        }
        else
        {
            g.setColour(HackAudio::Colours::Cyan);
        }
        juce::Rectangle<int>& r = pipAreas.getReference(i);
        g.fillEllipse(r.getX(), r.getY(), r.getWidth(), r.getHeight());
    }
}

void HackAudio::Slider::resized()
{

    int width  = getWidth();
    int height = getHeight();

    if (width == 0 || height == 0 || isResizing) { return; }

    isResizing = true;

    thumbArea.setSize(32, 32);

    if (getSliderStyle() == juce::Slider::LinearVertical)
    {

        sliderArea.setBounds(64, height / 6, 12, height - (height / 3));

        thumbArea.setX(sliderArea.getCentreX() - 16);

        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);
            r.setBounds(32, (sliderArea.getBottom() - 8) - (((sliderArea.getHeight()-8) / (pipAreas.size() - 1))) * i, 8, 8);
        }

        setMouseDragSensitivity(sliderArea.getHeight()/2);

        setSize(128, height);

    }
    else if (getSliderStyle() == juce::Slider::LinearHorizontal)
    {

        sliderArea.setBounds(0, 0, width - 128, 12);

        thumbArea.setY(12);

        for (juce::Rectangle<int>* r = pipAreas.begin(); r != pipAreas.end(); ++r)
        {

        }

        setMouseDragSensitivity(width/2);

        setSize(width, 128);

    }

    isResizing = false;

}

