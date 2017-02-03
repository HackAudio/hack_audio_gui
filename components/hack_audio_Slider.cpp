#include "hack_audio_Slider.h"

HackAudio::Slider::Slider()
{
    setSliderStyle(juce::Slider::LinearVertical);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setSliderSnapsToMousePosition(false);

    pipAreas.resize(10);

    animationAcc = 0;
    animationVel = 0;

    animationStart = animationEnd = juce::Point<int>(0, 0);

    isAnimating   = false;
    isResizing = false;

    addListener(this);

    setSize(128, 384);

    sliderValueChanged(this);
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
    if (trackArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {
        isAnimating = false;
        stopTimer();
        juce::Slider::mouseDown(e);
    }

    for (int i = 0; i < pipAreas.size(); ++i)
    {
        if (pipAreas[i].contains(e.getPosition()))
        {
            animationStart = thumbArea.getPosition();

            isAnimating = true;

            setValue((i / (float)pipAreas.size()) * getMaximum());

            if (getSliderStyle() == juce::Slider::LinearVertical)
            {
                animationEnd = thumbArea.getPosition().withY(trackArea.getBottom() - ((getValue()/getMaximum()) * trackArea.getHeight() + thumbArea.getHeight() / 2));
            }
            else if (getSliderStyle() == juce::Slider::LinearHorizontal)
            {
                animationEnd = thumbArea.getPosition().withX(trackArea.getX() + ((getValue()/getMaximum()) * trackArea.getWidth() - thumbArea.getHeight() / 2));
            }

            startTimerHz(60);
        }
    }
}

void HackAudio::Slider::timerCallback()
{
    if (getSliderStyle() == juce::Slider::LinearVertical)
    {
        if (thumbArea.getY() <= (animationStart.getY() + animationEnd.getY()) / 2)
        {
            animationAcc += ANIMATION_SPEED;
        }
        else
        {
            animationAcc -= ANIMATION_SPEED;
        }

        if (std::abs(animationVel) < 16)
        {
            animationVel += animationAcc;
        }

        thumbArea.translate(0, animationVel);
        highlightArea.setHeight(std::abs(trackArea.getBottom() - thumbArea.getCentreY()));
        highlightArea.setY(thumbArea.getCentreY());
    }
    else if (getSliderStyle() == juce::Slider::LinearHorizontal)
    {
        if (thumbArea.getX() <= (animationStart.getX() + animationEnd.getX()) / 2)
        {
            animationAcc += ANIMATION_SPEED;
        }
        else
        {
            animationAcc -= ANIMATION_SPEED;
        }

        if (std::abs(animationVel) < 16)
        {
            animationVel += animationAcc;
        }

        thumbArea.translate(animationVel, 0);
        highlightArea.setWidth(std::abs(trackArea.getX() - thumbArea.getCentreX()));
    }

    if (thumbArea.getPosition().getDistanceFrom(animationEnd) < 16)
    {
        animationAcc = 0;
        animationVel = 0;
        thumbArea.setPosition(animationEnd);
        isAnimating = false;
        stopTimer();
        repaint();
    }

    repaint();
}

void HackAudio::Slider::sliderValueChanged(juce::Slider*)
{
    if (!isAnimating)
    {
        if (getSliderStyle() == juce::Slider::LinearVertical)
        {
            thumbArea.setBounds(thumbArea.getX(), trackArea.getBottom() - ((getValue()/getMaximum()) * trackArea.getHeight() + thumbArea.getHeight() / 2), thumbArea.getWidth(), thumbArea.getHeight());

            highlightArea.setBounds(trackArea.getX(), thumbArea.getCentreY(), trackArea.getWidth(), std::abs(trackArea.getBottom() - thumbArea.getCentreY()));
        }
        else if (getSliderStyle() == juce::Slider::LinearHorizontal)
        {
            thumbArea.setBounds(trackArea.getX() + ((getValue()/getMaximum()) * trackArea.getWidth() - thumbArea.getHeight() / 2), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

            highlightArea.setBounds(trackArea.getX(), trackArea.getY(), std::abs(trackArea.getX() - thumbArea.getCentreX()), trackArea.getHeight());
        }
    }
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
    g.fillRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8);

    g.setColour(HackAudio::Colours::Cyan);
    g.fillRoundedRectangle(highlightArea.getX(), highlightArea.getY(), highlightArea.getWidth(), highlightArea.getHeight(), 8);

    g.setColour(HackAudio::Colours::White);
    g.fillEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

    g.setColour(HackAudio::Colours::Black);
    g.drawEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8);

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

    if ((width == 0 && height == 0) || isResizing) { return; }

    isResizing = true;

    thumbArea.setSize(32, 32);

    if (getSliderStyle() == juce::Slider::LinearVertical)
    {

        trackArea.setBounds(74, height / 6, 12, height - (height / 3));
        highlightArea.setWidth(trackArea.getWidth());
        thumbArea.setX(trackArea.getCentreX() - 16);
        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);
            r.setBounds(32, (trackArea.getBottom() - 8) - ((((float)(trackArea.getHeight() - 8) / (float)(pipAreas.size() - 1))) * i), 8, 8);
        }

        setSize(128, height);

    }
    else if (getSliderStyle() == juce::Slider::LinearHorizontal)
    {

        trackArea.setBounds(width / 6, 74, width - (width / 3), 12);
        highlightArea.setHeight(trackArea.getHeight());
        highlightArea.setPosition(trackArea.getPosition());
        thumbArea.setY(trackArea.getCentreY() - 16);

        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);
            r.setBounds(trackArea.getX() + ((float)(trackArea.getWidth() - 8) / (float)(pipAreas.size() - 1)) * i, 32, 8, 8);
        }

        setSize(width, 128);

    }

    isResizing = false;

}

