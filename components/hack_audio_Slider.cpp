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

    for (int i = 0; i <= pipAreas.size(); ++i)
    {
        if (pipAreas[i].contains(e.getPosition()))
        {
            animationStart = thumbArea.getPosition();

            isAnimating = true;

            setValue((i / (float)(pipAreas.size() - 1) * getMaximum()));

            if (isVertical())
            {
                animationEnd = thumbArea.getPosition().withY(thumbSpan.getY() - (getValue()/getMaximum()) * thumbSpan.getHeight());
            }
            else if (isHorizontal())
            {
                animationEnd = thumbArea.getPosition().withX(thumbSpan.getX() + (getValue()/getMaximum()) * thumbSpan.getWidth());
            }

            startTimerHz(60);
        }
    }
}

void HackAudio::Slider::timerCallback()
{
    if (isVertical())
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
        indicatorArea.setHeight(std::abs(trackArea.getBottom() - thumbArea.getCentreY()));
        indicatorArea.setY(thumbArea.getCentreY());
    }
    else if (isHorizontal())
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
        indicatorArea.setWidth(std::abs(trackArea.getX() - thumbArea.getCentreX()));
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
        if (isVertical())
        {
            thumbArea.setBounds(thumbArea.getX(), thumbSpan.getY() - ((getValue()/getMaximum()) * thumbSpan.getHeight()), thumbArea.getWidth(), thumbArea.getHeight());

            indicatorArea.setBounds(trackArea.getX(), thumbArea.getCentreY(), trackArea.getWidth(), std::abs(trackArea.getBottom() - thumbArea.getCentreY()));
        }
        else if (isHorizontal())
        {
            thumbArea.setBounds(thumbSpan.getX() + ((getValue()/getMaximum()) * thumbSpan.getWidth()), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

            indicatorArea.setBounds(trackArea.getX(), trackArea.getY(), std::abs(trackArea.getX() - thumbArea.getCentreX()), trackArea.getHeight());
        }
        else if (isRotary())
        {
            int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

            float angle = ((7.0f * M_PI) / 6.0f) + ((((getValue()/getMaximum())) * 10.0f) * (M_PI / 6.0f));

            juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
            indicatorArea.setCentre(destination.x, destination.y);
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

    // Draw Slider Track And Indicator
    if (isVertical() || isHorizontal())
    {
        g.setColour(HackAudio::Colours::Gray);
        g.fillRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8);

        g.setColour(HackAudio::Colours::Cyan);
        g.fillRoundedRectangle(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight(), 8);
    }

    // Draw Thumb Area And Indicator
    g.setColour(HackAudio::Colours::White);
    g.fillEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

    if (!isRotary())
    {
        g.setColour(HackAudio::Colours::Black);
        g.drawEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8);
    }
    else
    {
        g.setColour(HackAudio::Colours::Black);
        g.fillEllipse(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight());
    }

    // Draw Slider Pips
    for (int i = 0; i < pipAreas.size(); ++i)
    {
        if ((i / (float)pipAreas.size()) - (getValue() / getMaximum()) > 0)
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

    if (isVertical())
    {

        trackArea.setBounds(74, height / 6, 12, height - (height / 3));
        indicatorArea.setWidth(trackArea.getWidth());

        thumbArea.setX(trackArea.getCentreX() - 16);
        thumbSpan.setBounds(trackArea.getX(), trackArea.getBottom() - thumbArea.getHeight() / 2, trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);
            r.setBounds(32, (trackArea.getBottom() - 8) - ((((float)(trackArea.getHeight() - 8) / (float)(pipAreas.size() - 1))) * i), 8, 8);
        }

        setSize(128, height);

        setMouseDragSensitivity(trackArea.getHeight());

    }
    else if (isHorizontal())
    {

        trackArea.setBounds(width / 6, 74, width - (width / 3), 12);
        indicatorArea.setHeight(trackArea.getHeight());
        indicatorArea.setPosition(trackArea.getPosition());

        thumbArea.setY(trackArea.getCentreY() - 16);
        thumbSpan.setBounds(trackArea.getX() - thumbArea.getWidth() / 2, trackArea.getY(), trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);
            r.setBounds(trackArea.getX() + ((float)(trackArea.getWidth() - 8) / (float)(pipAreas.size() - 1)) * i, 32, 8, 8);
        }

        setSize(width, 128);

        setMouseDragSensitivity(trackArea.getWidth());

    }
    else if (isRotary())
    {

        trackArea.setBounds(0, 0, 0, 0);

        thumbArea.setBounds(32, 32, 64, 64);
        indicatorArea.setSize(8, 8);

        for (int i = 0; i < pipAreas.size(); ++i)
        {
            juce::Rectangle<int>& r = pipAreas.getReference(i);

            int radius = sqrt ( pow(thumbArea.getWidth() / 2, 2) * 2);

            float angle = ((7.0f * M_PI) / 6.0f) + (((i/9.0f) * 10.0f) * (M_PI / 6.0f));

            juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
            r.setCentre(destination.x, destination.y);
        }

        setSize(128, 128);

        setMouseDragSensitivity(2 * M_PI * 32);

    }

    isResizing = false;

}

