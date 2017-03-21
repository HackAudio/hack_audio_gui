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
    
    pipsShown     = true;
    pipSize       = 0;
    minPipSize    = DEFAULT_PIPMIN;
    maxPipSize    = DEFAULT_PIPMAX;

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

void HackAudio::Slider::setPipState(bool shouldBeShown)
{
    pipsShown = shouldBeShown;
    resized();
    repaint();
}

bool HackAudio::Slider::getPipState()
{
    return pipsShown;
}

void HackAudio::Slider::setPipCount(int count)
{
    count = std::max(2, count);
    pipLocations.resize(count);
    setPipScale();
    resized();
    repaint();
}

int HackAudio::Slider::getPipCount()
{
    return pipLocations.size();
}

void HackAudio::Slider::setPipSize(int minSize, int maxSize)
{
    minPipSize = minSize;
    maxPipSize = maxSize;
    setPipScale();
    resized();
    repaint();
}

void HackAudio::Slider::resetPipSize()
{
    minPipSize = DEFAULT_PIPMIN;
    maxPipSize = DEFAULT_PIPMAX;
    setPipScale();
    resized();
    repaint();
}

void HackAudio::Slider::setPipScale()
{
    int pipSizeCheck;

    if (!isRotary())
    {
        pipSizeCheck = trackArea.getHeight() / (pipLocations.size() - 1);

        if (pipSizeCheck < maxPipSize * 2)
        {
            currentMinPipSize *= pipSizeCheck/ maxPipSize;
            currentMaxPipSize = pipSizeCheck;
        }
        else
        {
            currentMinPipSize = minPipSize;
            currentMaxPipSize = maxPipSize;
        }
    }
    else
    {
        int radius = sqrt (pow(thumbArea.getWidth() / 2, 2) * 2);

        float offset = ROTARY_ANGLERANGE / pipLocations.size();
        float angleOne = ROTARY_ANGLESTART;
        float angleTwo = ROTARY_ANGLESTART + offset;

        juce::Point<float> pointOne = thumbArea.getCentre().getPointOnCircumference(radius, angleOne);
        juce::Point<float> pointTwo = thumbArea.getCentre().getPointOnCircumference(radius, angleTwo);

        pipSizeCheck = pointOne.getDistanceFrom(pointTwo);

        if (pipSizeCheck < maxPipSize)
        {
            currentMinPipSize *= pipSizeCheck/ maxPipSize;
            currentMaxPipSize = pipSizeCheck;
        }
        else
        {
            currentMinPipSize = minPipSize;
            currentMaxPipSize = maxPipSize;
        }
    }


}

void HackAudio::Slider::setSymmetricSize(int size)
{
    setSize(size, size);
}

void HackAudio::Slider::mouseDown(const juce::MouseEvent &e)
{
    if (trackArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {

        stopAnimation();

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
            if (pipLocations[i].getDistanceFrom(e.getPosition()) <= maxPipSize * 0.75f)
            {
                stopAnimation();
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
        if (pipLocations[i].getDistanceFrom(e.getPosition()) <= maxPipSize * 0.75f)
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

                float offset = ROTARY_ANGLERANGE / (pipLocations.size() - 1);
                float angle = ROTARY_ANGLESTART + (i * offset);

                float startAngle = thumbArea.getCentre().getAngleToPoint(indicatorArea.getCentre());
                juce::Point<float> start = thumbArea.getCentre().getPointOnCircumference(radius, startAngle);
                animationStart.setXY(start.x, start.y);

                juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
                animationEnd.setXY(destination.x, destination.y);

            }

            startTimerHz(ANIMATION_FPS);
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
        ((!isRotary()) && (thumbArea.getPosition().getDistanceFrom(animationEnd) <= 16)) ||
        ((isRotary()) && (indicatorArea.getCentre().getDistanceFrom(animationEnd) <= 8))
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

            float angle = ROTARY_ANGLESTART + (ROTARY_ANGLERANGE * ((getValue() - getMinimum()) / (getMaximum() - getMinimum())));

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

void HackAudio::Slider::stopAnimation()
{
    isAnimating = false;
    animationAcc = 0;
    animationVel = 0;
    stopTimer();
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
    for (int i = 0; i < pipLocations.size() && pipsShown; ++i)
    {

        pipSize = minPipSize;

        if (isVertical())
        {

            if (thumbArea.getY() > pipLocations[i].getY())
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = currentMinPipSize;
            }
            else
            {
                g.setColour(HackAudio::Colours::Cyan);

                int diff = ((pipLocations[i].getY() - thumbArea.getY()));
                if (diff > currentMaxPipSize && diff > 0)
                {
                    pipSize = currentMaxPipSize;
                }
                else
                {
                    pipSize = diff;
                }
            }

        }
        else if (isHorizontal())
        {

            if (thumbArea.getX() + thumbArea.getWidth() < pipLocations[i].getX())
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = currentMinPipSize;
            }
            else
            {
                g.setColour(HackAudio::Colours::Cyan);

                float diff = ((thumbArea.getX() + thumbArea.getWidth() - pipLocations[i].getX()));

                if (diff > currentMaxPipSize && diff > 0)
                {
                    pipSize = currentMaxPipSize;
                }
                else
                {
                    pipSize = diff;
                }
            }

        }
        else if (isRotary())
        {

            float angle = thumbArea.getCentre().getAngleToPoint(pipLocations[i-1]);
            float indicatorAngle = thumbArea.getCentre().getAngleToPoint(indicatorArea.getCentre());

            if (angle >= indicatorAngle)
            {
                g.setColour(HackAudio::Colours::Gray);
                pipSize = currentMinPipSize;
            }
            else
            {
                float diff = angle - indicatorAngle;
                if (diff <= 0 && diff > -0.4)
                {
                    g.setColour(HackAudio::Colours::Cyan);
                    pipSize = currentMaxPipSize * (-diff + 0.2);
                }
                else
                {
                    g.setColour(HackAudio::Colours::Cyan);
                    pipSize = currentMaxPipSize;
                }
            }

            if (i == 0)
            {
                g.setColour(HackAudio::Colours::Cyan);
                pipSize = currentMaxPipSize;
            }

        }

        juce::Point<int>& p = pipLocations.getReference(i);


        g.fillEllipse(p.getX() - pipSize/2, p.getY() - pipSize/2, pipSize, pipSize);
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

        if (height == 0) { resizeGuard = false; return; }

        int trackOffset = (pipsShown) ? (width / 2) + 10 : (width / 2) - 6;

        trackArea.setBounds(trackOffset, height / 6, 12, height - (height / 3));
        indicatorArea.setWidth(trackArea.getWidth());
        indicatorArea.setX(trackArea.getX());

        thumbArea.setX(trackArea.getCentreX() - 16);
        thumbSpan.setBounds(trackArea.getX(), trackArea.getBottom() - thumbArea.getHeight() / 2, trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY(trackArea.getX() - 42, (trackArea.getBottom()) - ((((float)(trackArea.getHeight()) / (float)(pipLocations.size() - 1)) * i)));
        }

        setSize(width, height);

        setMouseDragSensitivity(trackArea.getHeight());

    }
    else if (isHorizontal())
    {

        if (width == 0) { resizeGuard = false; return; }

        int trackOffset = (pipsShown) ? (height / 2) + 10 : (height / 2) - 6;

        trackArea.setBounds(width / 6, trackOffset, width - (width / 3), 12);
        indicatorArea.setHeight(trackArea.getHeight());
        indicatorArea.setPosition(trackArea.getPosition());

        thumbArea.setY(trackArea.getCentreY() - 16);
        thumbSpan.setBounds(trackArea.getX() - thumbArea.getWidth() / 2, trackArea.getY(), trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY(trackArea.getX() + ((float)(trackArea.getWidth()) / (float)(pipLocations.size() - 1)) * i, trackArea.getY() - 34);
        }

        setSize(width, height);

        setMouseDragSensitivity(trackArea.getWidth());

    }
    else if (isRotary())
    {

        if (width != height)
        {
            int size;
            size = std::min(width, height);
            width = height = size;
        }

        trackArea.setBounds(0, 0, 0, 0);

        thumbArea.setBounds(width / 4, height / 4, width / 2, height / 2);

        int indicatorSize = std::min(thumbArea.getWidth() / (DEFAULT_PIPMAX / 2), DEFAULT_PIPMAX);
        indicatorArea.setSize(indicatorSize, indicatorSize);

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);

            int radius = sqrt ( pow(thumbArea.getWidth() / 2, 2) * 2);

            float offset = ROTARY_ANGLERANGE / (pipLocations.size() - 1);
            float angle = ROTARY_ANGLESTART + (i * offset);

            juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
            p.setXY(destination.x, destination.y);
        }

        int radius = (thumbArea.getWidth() / 2) - (thumbArea.getWidth() / 8);

        float offset = ROTARY_ANGLERANGE * ((getValue() - getMinimum()) / (getMaximum() - getMinimum()));
        float angle = ROTARY_ANGLESTART + offset;

        juce::Point<float> destination = thumbArea.getCentre().getPointOnCircumference(radius, angle);
        indicatorArea.setCentre(destination.x, destination.y);

        setSize(width, height);

        setMouseDragSensitivity(2 * M_PI * 32);

    }

    setPipScale();
    sliderValueChanged(this);

    resizeGuard = false;

}

