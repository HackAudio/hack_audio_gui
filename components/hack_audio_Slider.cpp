/* Copyright (C) 2017 by Antonio Lassandro, HackAudio LLC
 *
 * hack_audio_gui is provided under the terms of The MIT License (MIT):
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

HackAudio::Slider::Slider()
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    resizeGuard = true;
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    resizeGuard = false;

    setSliderSnapsToMousePosition(false);
    setDoubleClickReturnValue(false, 0);
    setRepaintsOnMouseActivity(false);

    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);

    pipLocations.resize(10);

    animationAcc = 0;
    animationVel = 0;

    animationStart = animationEnd = juce::Point<int>(0, 0);

    isAnimating   = false;
    resizeGuard   = false;
    isDraggable   = false;
    isSettable    = false;
    
    pipsShown     = true;
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

bool HackAudio::Slider::getPipState() const
{

    return pipsShown;

}

void HackAudio::Slider::setPipCount(int count)
{

    count = std::max(2, count);
    pipLocations.resize(count);
    resized();
    repaint();

}

int HackAudio::Slider::getPipCount() const
{

    return pipLocations.size();

}

void HackAudio::Slider::setPipScale()
{

    int pipSizeCheck;

    if (!isRotary())
    {

        maxPipSize = thumbArea.getWidth() / 4;
        minPipSize = currentMaxPipSize * 0.75f;

        if (isVertical())
        {
            pipSizeCheck = trackArea.getHeight() / (pipLocations.size() - 1);
        }
        else
        {
            pipSizeCheck = trackArea.getWidth() / (pipLocations.size() - 1);
        }

        if (pipSizeCheck < maxPipSize)
        {
            currentMinPipSize *= pipSizeCheck / maxPipSize;
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

        maxPipSize = thumbArea.getWidth() / 8;
        minPipSize = currentMaxPipSize * 0.75f;

        int radius = sqrt (pow(thumbArea.getWidth() / 2, 2) * 2);

        float offset = ROTARY_ANGLERANGE / pipLocations.size();
        float angleOne = ROTARY_ANGLESTART;
        float angleTwo = ROTARY_ANGLESTART + offset;

        juce::Point<float> pointOne = thumbArea.getCentre().getPointOnCircumference(radius, angleOne);
        juce::Point<float> pointTwo = thumbArea.getCentre().getPointOnCircumference(radius, angleTwo);

        pipSizeCheck = pointOne.getDistanceFrom(pointTwo);

        if (pipSizeCheck < maxPipSize)
        {
            currentMinPipSize *= pipSizeCheck / maxPipSize;
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

void HackAudio::Slider::mouseMove(const juce::MouseEvent& e)
{

    if (!isEnabled()) { return; }

    if (trackArea.contains(e.getPosition()) || thumbArea.contains(e.getPosition()))
    {

        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        return;

    }
    else
    {

        for (int i = 0; i <= pipLocations.size(); ++i)
        {

            if (pipLocations[i].getDistanceFrom(e.getPosition()) <= maxPipSize * 0.75f)
            {

                setMouseCursor(juce::MouseCursor::PointingHandCursor);
                return;

            }
            
        }
        
    }
    
    setMouseCursor(juce::MouseCursor::NormalCursor);
    
}

void HackAudio::Slider::mouseDown(const juce::MouseEvent &e)
{

    pipClicked = -1;

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

                pipClicked = i;

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

void HackAudio::Slider::focusGained(juce::Component::FocusChangeType cause)
{

    setColour(HackAudio::midgroundColourId, HackAudio::Colours::LightGray);
    repaint();

}

void HackAudio::Slider::focusLost(juce::Component::FocusChangeType cause)
{

    setColour(HackAudio::midgroundColourId, HackAudio::Colours::Gray);
    repaint();

}

void HackAudio::Slider::enablementChanged()
{

    if (isEnabled())
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
        setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
        setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    }
    else
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
        setColour(HackAudio::foregroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::highlightColourId,  HackAudio::Colours::Black);

    }

    repaint();

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
        if (pipLocations[i].getDistanceFrom(e.getPosition()) <= currentMaxPipSize)
        {

            if (i != pipClicked) { return; }

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

bool HackAudio::Slider::keyPressed(const juce::KeyPress &key)
{

    if (key.getKeyCode() == key.escapeKey)
    {

        juce::Component::unfocusAllComponents();
        return true;

    }

    float value    = getValue();
    float interval = (getInterval()) ? getInterval() : (getMaximum() - getMinimum()) / pipLocations.size();

    if (key.getModifiers().isShiftDown())
    {

        interval *= 2;

    }

    if (isHorizontal())
    {

        if (key.getKeyCode() == key.leftKey)
        {

            setValue(value - interval);
            return true;

        }

        if (key.getKeyCode() == key.leftKey)
        {

            setValue(value + interval);
            return true;

        }

    }

    if (isVertical())
    {

        if (key.getKeyCode() == key.upKey)
        {

            setValue(value + interval);
            return true;

        }

        if (key.getKeyCode() == key.downKey)
        {

            setValue(value - interval);
            return true;

        }

    }

    if (isRotary())
    {

        if (key.getKeyCode() == key.upKey)
        {

            setValue(value + interval);
            return true;

        }

        if (key.getKeyCode() == key.downKey)
        {

            setValue(value - interval);
            return true;

        }

        if (key.getKeyCode() == key.leftKey)
        {

            setValue(value - interval);
            return true;

        }

        if (key.getKeyCode() == key.rightKey)
        {

            setValue(value + interval);
            return true;

        }

    }

    return false;

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
    p.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(p);

    // Draw Slider Track And Indicator
    if (isVertical() || isHorizontal())
    {
        g.setColour(findColour(HackAudio::backgroundColourId));
        g.fillRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8);

        g.setColour(findColour(HackAudio::highlightColourId));
        g.fillRoundedRectangle(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight(), 8);

    }

    // Draw Thumb Area And Indicator
    g.setColour(findColour(HackAudio::foregroundColourId));
    g.fillEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight());

    if (!isRotary())
    {

        g.setColour(findColour(HackAudio::midgroundColourId));
        g.drawEllipse(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), thumbArea.getWidth() / 4);

    }
    else
    {

        g.setColour(findColour(HackAudio::midgroundColourId));
        g.fillEllipse(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight());

    }

    // Draw Slider Pips
    for (int i = 0; i < pipLocations.size() && pipsShown; ++i)
    {

        int pipSize = minPipSize;

        if (isVertical())
        {

            if (thumbArea.getY() > pipLocations[i].getY())
            {

                g.setColour(findColour(HackAudio::backgroundColourId));
                pipSize = currentMinPipSize;

            }
            else
            {

                g.setColour(findColour(HackAudio::highlightColourId));

                int diff = ((pipLocations[i].getY() - thumbArea.getY()));

                if (diff > currentMaxPipSize && diff > 0)
                {
                    pipSize = currentMaxPipSize;
                }
                else
                {
                    pipSize = std::max(currentMinPipSize / 3, diff);
                }

            }

        }
        else if (isHorizontal())
        {

            if (thumbArea.getX() + thumbArea.getWidth() < pipLocations[i].getX())
            {

                g.setColour(findColour(HackAudio::backgroundColourId));
                pipSize = currentMinPipSize;

            }
            else
            {

                g.setColour(findColour(HackAudio::highlightColourId));

                int diff = ((thumbArea.getX() + thumbArea.getWidth() - pipLocations[i].getX()));

                if (diff > currentMaxPipSize && diff > 0)
                {
                    pipSize = currentMaxPipSize;
                }
                else
                {
                    pipSize = std::max(currentMinPipSize / 3, diff);
                }

            }

        }
        else if (isRotary())
        {

            float angle = thumbArea.getCentre().getAngleToPoint(pipLocations[i-1]);
            float indicatorAngle = thumbArea.getCentre().getAngleToPoint(indicatorArea.getCentre());

            if (angle >= indicatorAngle || (!isAnimating && getValue() == getMinimum()))
            {

                g.setColour(findColour(HackAudio::backgroundColourId));
                pipSize = currentMinPipSize;

            }
            else
            {

                float diff = angle - indicatorAngle;

                if (diff <= 0 && diff > -0.4)
                {
                    g.setColour(findColour(HackAudio::highlightColourId));
                    pipSize = std::max(currentMaxPipSize / 3.0, currentMaxPipSize * (-diff + 0.2));
                }
                else
                {
                    g.setColour(findColour(HackAudio::highlightColourId));
                    pipSize = currentMaxPipSize;
                }

            }

            if (i == 0)
            {
                g.setColour(findColour(HackAudio::highlightColourId));
                pipSize = currentMaxPipSize;
            }

            if (!isAnimating && getValue() == getMaximum())
            {
                g.setColour(findColour(HackAudio::highlightColourId));
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

    if (isVertical())
    {

        if (height == 0) { resizeGuard = false; return; }

        trackArea.setWidth(width / (10 + 2/3));
        trackArea.setHeight(height - (height / 3));

        if (pipsShown)
        {
            trackArea.setPosition((width / 2) + trackArea.getWidth() / (1 + 1/5), height / 6);
        }
        else
        {
            trackArea.setPosition((width / 2) - trackArea.getWidth() / 2, height / 6);
        }

        indicatorArea.setWidth(trackArea.getWidth());
        indicatorArea.setX(trackArea.getX());

        thumbArea.setSize((float)trackArea.getWidth() * (2.0f + 2.0f/3.0f), (float)trackArea.getWidth() * (2.0f + 2.0f/3.0f));

        thumbArea.setX(trackArea.getCentreX() - thumbArea.getWidth() / 2);
        thumbSpan.setBounds(trackArea.getX(), trackArea.getBottom() - thumbArea.getHeight() / 2, trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY((float)trackArea.getX() - ((float)trackArea.getWidth() * 3.5f), (trackArea.getBottom()) - ((((float)(trackArea.getHeight()) / (float)(pipLocations.size() - 1)) * i)));
        }

        setSize(width, height);

        setMouseDragSensitivity(trackArea.getHeight());

    }
    else if (isHorizontal())
    {

        if (width == 0) { resizeGuard = false; return; }

        trackArea.setWidth(width - (width / 3));
        trackArea.setHeight(height / (10 + 2/3));

        if (pipsShown)
        {

            trackArea.setPosition(width / 6, (height / 2) + trackArea.getHeight() / (1 + 1/5));

        }
        else
        {

            trackArea.setPosition(width / 6, (height / 2) + trackArea.getHeight() / 2);

        }

        indicatorArea.setHeight(trackArea.getHeight());
        indicatorArea.setPosition(trackArea.getPosition());

        thumbArea.setSize((float)trackArea.getHeight() * (2.0f + 2.0f/3.0f), (float)trackArea.getHeight() * (2.0f + 2.0f/3.0f));

        thumbArea.setY(trackArea.getCentreY() - thumbArea.getWidth() / 2);
        thumbSpan.setBounds(trackArea.getX() - thumbArea.getWidth() / 2, trackArea.getY(), trackArea.getWidth(), trackArea.getHeight());

        for (int i = 0; i < pipLocations.size(); ++i)
        {
            juce::Point<int>& p = pipLocations.getReference(i);
            p.setXY(trackArea.getX() + ((float)(trackArea.getWidth()) / (float)(pipLocations.size() - 1)) * i, trackArea.getY() - ((float)trackArea.getHeight() * 3.5f));
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

        int indicatorSize = thumbArea.getWidth() / (DEFAULT_PIPMAX * 0.85f);
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
