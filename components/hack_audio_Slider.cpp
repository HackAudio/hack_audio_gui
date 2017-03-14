#include "hack_audio_Slider.h"

HackAudio::Slider::Slider()
{
    resizeGuard = true;
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    resizeGuard = false;

    setSliderSnapsToMousePosition(false);
    setDoubleClickReturnValue(false, 0);

    pipLocations.resize(10);

    animationAcc = 0;
    animationVel = 0;

    animationStart = animationEnd = juce::Point<int>(0, 0);

    isAnimating   = false;
    resizeGuard   = false;
    isDraggable   = false;
    isSettable    = false;

    addListener(this);

    setSize(128, 384);
}

HackAudio::Slider::~Slider()
{

}

void HackAudio::Slider::setDefaultValue(bool shouldHaveDefault, double defaultValue)
{
    hasDefault = shouldHaveDefault;
    sliderDefault = defaultValue;
}

void HackAudio::Slider::mouseDown(const juce::MouseEvent &e)
{
    if (trackArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {
        if (e.mods.isAltDown() && hasDefault)
        {
            setValue(sliderDefault);
            return;
        }

        isDraggable = true;
        isSettable  = false;
        juce::Slider::mouseDown(e);
    }
    else
    {

        isSettable = false;

        for (int i = 0; i <= pipLocations.size(); ++i)
        {
            if (pipLocations[i].getDistanceFrom(e.getPosition()) <= 16)
            {
                isSettable = true;
            }
        }

        isDraggable = false;
        return;
    }
}

void HackAudio::Slider::mouseDrag(const juce::MouseEvent& e)
{
    if (isDraggable)
    {
        juce::Slider::mouseDrag(e);
    }
    else
    {
        return;
    }
}

void HackAudio::Slider::mouseUp(const juce::MouseEvent& e)
{
    if (trackArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {
        isAnimating = false;
        stopTimer();
        juce::Slider::mouseUp(e);
        return;
    }

    if (isSettable == false) { return; }

    for (int i = 0; i <= pipLocations.size(); ++i)
    {
        if (pipLocations[i].getDistanceFrom(e.getPosition()) <= 16)
        {

            if (std::abs((i / (float)(pipLocations.size() - 1.0f) * (getMaximum() - getMinimum()) + getMinimum()) - getValue()) > 0.01f)
            {
                setValue((i / (float)(pipLocations.size() - 1.0f)) * (getMaximum() - getMinimum()) + getMinimum());
            }
            else
            {
                return;
            }

            animationStart = thumbArea.getPosition();

            isAnimating = true;

            if (isVertical())
            {
                animationEnd = thumbArea.getPosition().withY(thumbSpan.getY() - ((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * thumbSpan.getHeight());
            }
            else if (isHorizontal())
            {
                animationEnd = thumbArea.getPosition().withX(thumbSpan.getX() + ((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * thumbSpan.getWidth());
            }
            else if (isRotary())
            {
                int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

                float angle = ((7.0f * M_PI) / 6.0f) + (((i/9.0f) * 10.0f) * (M_PI / 6.0f));

                float startAngle = thumbArea.getCentre().getAngleToPoint(indicatorArea.getCentre());
                juce::Point<float> start = thumbArea.getCentre().getPointOnCircumference(radius, startAngle);
                animationStart.setXY(start.x, start.y);

                juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
                animationEnd.setXY(destination.x, destination.y);

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
    else if (isRotary())
    {
        int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

        float startAngle   = thumbArea.getCentre().getAngleToPoint(animationStart);
        float targetAngle  = thumbArea.getCentre().getAngleToPoint(animationEnd);
        float currentAngle = thumbArea.getCentre().getAngleToPoint(indicatorArea.getCentre());

        if (currentAngle <= (startAngle + targetAngle) / 2)
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


        currentAngle += (animationVel * M_PI) / 180;
        juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, currentAngle);

        indicatorArea.setCentre(destination.x, destination.y);

    }

    if (
        ((!isRotary()) && (thumbArea.getPosition().getDistanceFrom(animationEnd) < 16)) ||
        ((isRotary()) && (indicatorArea.getCentre().getDistanceFrom(animationEnd) < 4))
        )
    {
        animationAcc = 0;
        animationVel = 0;


        (isRotary()) ? indicatorArea.setCentre(animationEnd) : thumbArea.setPosition(animationEnd);


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
            thumbArea.setBounds(thumbArea.getX(), thumbSpan.getY() - (((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * thumbSpan.getHeight()), thumbArea.getWidth(), thumbArea.getHeight());

            indicatorArea.setBounds(trackArea.getX(), thumbArea.getCentreY(), trackArea.getWidth(), std::abs(trackArea.getBottom() - thumbArea.getCentreY()));
        }
        else if (isHorizontal())
        {
            thumbArea.setBounds(thumbSpan.getX() + (((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * thumbSpan.getWidth()), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

            indicatorArea.setBounds(trackArea.getX(), trackArea.getY(), std::abs(trackArea.getX() - thumbArea.getCentreX()), trackArea.getHeight());
        }
        else if (isRotary())
        {
            int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

            float angle = ((7.0f * M_PI) / 6.0f) + (((((getValue() - getMinimum()) / (getMaximum() - getMinimum()))) * 10.0f) * (M_PI / 6.0f));

            juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
            indicatorArea.setCentre(destination.x, destination.y);
        }
    }
}

void HackAudio::Slider::parentHierarchyChanged()
{
    resized();
    sliderValueChanged(this);
}

void HackAudio::Slider::paint(juce::Graphics& g)
{
    int width = getWidth();
    int height = getHeight();

    // Draw Background
    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
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
        g.setColour(HackAudio::Colours::Gray);
        g.fillEllipse(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight());
    }

    // Draw Slider Pips
    for (int i = 0; i < pipLocations.size(); ++i)
    {

        int minPipSize = 6;
        int maxPipSize = 8;

        int pipSize;

        if (isVertical())
        {

            if (thumbArea.getY() - thumbArea.getWidth() / 2 > pipLocations[i].getY())
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = minPipSize;
            }
            else
            {
                g.setColour(HackAudio::Colours::Cyan);
                pipSize = maxPipSize;
            }

        }
        else if (isHorizontal())
        {

            if (thumbArea.getX() + thumbArea.getWidth() / 2 < pipLocations[i].getX())
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = minPipSize;
            }
            else
            {
                g.setColour(HackAudio::Colours::Cyan);
                pipSize = maxPipSize;
            }

        }
        else if (isRotary())
        {

            if ((i / (float)(pipLocations.size() - 1)) - 0.025 > ((getValue() - getMinimum()) / (getMaximum() - getMinimum())))
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = minPipSize;
            }
            else
            {
                g.setColour(HackAudio::Colours::Cyan);

                float diff = (i / (float)(pipLocations.size() - 1)) - ((getValue() - getMinimum()) / (getMaximum() - getMinimum()));

                if (diff > 0)
                {
                    pipSize = maxPipSize - (maxPipSize * diff);
                }
                else
                {
                    pipSize = maxPipSize;
                }
            }
        }

        juce::Point<int>& p = pipLocations.getReference(i);

        if (!isRotary())
        {
            g.fillEllipse(p.getX() - (pipSize - minPipSize), p.getY() - (pipSize - minPipSize), pipSize, pipSize);
        }
        else
        {
            g.fillEllipse(p.getX() - pipSize/2, p.getY() - pipSize/2, pipSize, pipSize);
        }
    }
}

void HackAudio::Slider::resized()
{

    int width  = getWidth();
    int height = getHeight();

    if (resizeGuard) { return; }

    resizeGuard = true;

    thumbArea.setSize(32, 32);

    if (isVertical())
    {

        if (height == 0) { return; }

        trackArea.setBounds(74, height / 6, 12, height - (height / 3));
        indicatorArea.setWidth(trackArea.getWidth());

        thumbArea.setX(trackArea.getCentreX() - 16);
        thumbSpan.setBounds(trackArea.getX(), trackArea.getBottom() - thumbArea.getHeight() / 2, trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY(32, (trackArea.getBottom() - 8) - ((((float)(trackArea.getHeight() - 8) / (float)(pipLocations.size() - 1)))));
        }

        setSize(128, height);

        setMouseDragSensitivity(trackArea.getHeight());

    }
    else if (isHorizontal())
    {

        if (width == 0) { return; }

        trackArea.setBounds(width / 6, 74, width - (width / 3), 12);
        indicatorArea.setHeight(trackArea.getHeight());
        indicatorArea.setPosition(trackArea.getPosition());

        thumbArea.setY(trackArea.getCentreY() - 16);
        thumbSpan.setBounds(trackArea.getX() - thumbArea.getWidth() / 2, trackArea.getY(), trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY(trackArea.getX() + ((float)(trackArea.getWidth() - 8) / (float)(pipLocations.size() - 1)) * i, 32);
        }

        setSize(width, 128);

        setMouseDragSensitivity(trackArea.getWidth());

    }
    else if (isRotary())
    {

        trackArea.setBounds(0, 0, 0, 0);

        thumbArea.setBounds(32, 32, 64, 64);
        indicatorArea.setSize(8, 8);

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);

            int radius = sqrt ( pow(thumbArea.getWidth() / 2, 2) * 2);

            float angle = ((7.0f * M_PI) / 6.0f) + (((i/9.0f) * 10.0f) * (M_PI / 6.0f));

            juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
            p.setXY(destination.x, destination.y);


        }

        int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

        float angle = ((7.0f * M_PI) / 6.0f) + (((((getValue() - getMinimum()) / (getMaximum() - getMinimum()))) * 10.0f) * (M_PI / 6.0f));

        juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
        indicatorArea.setCentre(destination.x, destination.y);

        setSize(128, 128);

        setMouseDragSensitivity(2 * M_PI * 32);

    }

    resizeGuard = false;

}

