#include "hack_audio_Button.h"

HackAudio::Button::Button() : juce::Button("")
{
	setButtonText("");

	setClickingTogglesState(true);
	setTriggeredOnMouseDown(false);

	resizeGuard = false;

    currentColourInterpolation.reset(30, 0.45);
}

HackAudio::Button::~Button()
{

}

void HackAudio::Button::setButtonStyle(HackAudio::Button::ButtonStyle style)
{
    buttonStyle = style;
    resized();
    repaint();
}

HackAudio::Button::ButtonStyle HackAudio::Button::getButtonStyle()
{
    return buttonStyle;
}

void HackAudio::Button::setFont(juce::Font font)
{
    buttonFont = font;
    resized();
    repaint();
}

juce::Font HackAudio::Button::getFont()
{
    return buttonFont;
}

void HackAudio::Button::mouseDown(const juce::MouseEvent& e)
{

    currentColourInterpolation.setValue(1.0f);

    if (buttonStyle == ButtonStyle::BarSingleton)
    {

        startTimerHz(ANIMATION_FPS);
        setToggleState(true, juce::sendNotification);

    }
    else if (buttonStyle == ButtonStyle::BarToggle)
    {
        startTimerHz(ANIMATION_FPS);
        juce::Button::mouseDown(e);

    }
    else if (buttonStyle == ButtonStyle::SlidingToggle)
    {

        isDraggable = false;

        if (!trackArea.contains(e.getPosition()) && !thumbArea.contains(e.getPosition())) { return; }

        if (thumbArea.contains(e.getPosition()))
        {
            isDraggable = true;
        }

    }
}

void HackAudio::Button::mouseDrag(const juce::MouseEvent& e)
{

    if (buttonStyle != ButtonStyle::SlidingToggle)
    {

        juce::Button::mouseDrag(e);

    }
    else
    {

        if (!isDraggable || !e.mouseWasDraggedSinceMouseDown()) { return; }

        int xoffset = thumbArea.getX();

        if (e.x > 32 || e.x < 64)
        {
            thumbArea.setX(xoffset + (e.getDistanceFromDragStartX() / 8));
        }

        if (thumbArea.getX() < 32)
        {
            thumbArea.setX(32);
        }
        else if (thumbArea.getX() > 64)
        {
            thumbArea.setX(64);
        }

        indicatorArea.setWidth((thumbArea.getX() - indicatorArea.getX()) + thumbArea.getWidth()/2);

        repaint();

    }

}

void HackAudio::Button::mouseUp(const juce::MouseEvent& e)
{

    if (buttonStyle == ButtonStyle::BarSingleton)
    {

        setToggleState(false, juce::dontSendNotification);

    }
    else if (buttonStyle == ButtonStyle::BarToggle)
    {

        juce::Button::mouseUp(e);

    }
    else if (buttonStyle == ButtonStyle::SlidingToggle)
    {

        if (trackArea.contains(e.getPosition()))
        {

            animationStart.setXY(thumbArea.getX(), thumbArea.getY());

            if (e.x <= getWidth() / 2)
            {
                setToggleState(false, juce::sendNotification);
                animationEnd.setXY(32, 16);
            }
            else
            {
                setToggleState(true, juce::sendNotification);
                animationEnd.setXY(64, 16);
            }

            startTimerHz(60);

        }

    }
    
}

void HackAudio::Button::timerCallback()
{

    if (buttonStyle != ButtonStyle::SlidingToggle)
    {

        if (currentColourInterpolation.isSmoothing())
        {
            repaint();

            if (std::abs(currentColourInterpolation.getTargetValue() - currentColourInterpolation.getNextValue()) < 0.0001)
            {
                currentColourInterpolation.setValue(currentColourInterpolation.getTargetValue());
            }

        }
        else
        {
            if (currentColourInterpolation.getTargetValue() == 1.0f && !(juce::Desktop::getInstance().getDraggingMouseSource(0)))
            {
                currentColourInterpolation.setValue(0.0f);
            }
            else
            {
                if (currentColourInterpolation.getNextValue() == 0.0f)
                {
                    stopTimer();
                }
                return;
            }
        }

    }
    else
    {

        if (thumbArea.getX() <= (animationStart.getX() + animationEnd.getX()) / 2)
        {
            animationAcc += ANIMATION_SPEED;
        }
        else
        {
            animationAcc -= ANIMATION_SPEED;
        }

        if (std::abs(animationVel) < 4)
        {
            animationVel += animationAcc;
        }

        thumbArea.translate(animationVel, 0);
        indicatorArea.setWidth((thumbArea.getX() - indicatorArea.getX()) + thumbArea.getWidth()/2);

        if (thumbArea.getPosition().getDistanceFrom(animationEnd) < 4)
        {
            animationAcc = 0;
            animationVel = 0;

            thumbArea.setPosition(animationEnd);

            stopTimer();
            repaint();
        }

    }

	repaint();

}

void HackAudio::Button::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    int width  = getWidth();
    int height = getHeight();

    if (buttonStyle != ButtonStyle::SlidingToggle)
    {

        juce::Path p;
        p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, false, !(isConnectedOnTop() || isConnectedOnRight()), !(isConnectedOnBottom() || isConnectedOnLeft()), false);

        juce::Colour background;
        juce::Colour foreground;

        if (getToggleState() &&  buttonStyle != ButtonStyle::BarSingleton)
        {
            background = HackAudio::Colours::Cyan;
            foreground = HackAudio::Colours::White;
        }
        else
        {
            background = HackAudio::Colours::White;
            foreground = HackAudio::Colours::Gray;
        }

        g.setColour(background.interpolatedWith(HackAudio::Colours::Black, currentColourInterpolation.getNextValue()));

        g.fillPath(p);
        p.clear();

        g.setColour(foreground.interpolatedWith(HackAudio::Colours::Gray, currentColourInterpolation.getNextValue()));

        g.setFont(buttonFont);
        g.drawText(getButtonText(), 0, 0, width, height, juce::Justification::centred, 1);

    }
    else
    {

        juce::Path p;
        p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
        g.setColour(HackAudio::Colours::Black);
        g.fillPath(p);

        p.clear();

        p.addRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8, 8, false, true, true, false);
        g.setColour(HackAudio::Colours::Gray);
        g.fillPath(p);

        p.clear();

        p.addRoundedRectangle(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight(), 8, 8, false, true, true, false);
        g.setColour(HackAudio::Colours::Cyan);
        g.fillPath(p);

        p.clear();

        p.addRoundedRectangle(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8, 8, false, true, true, false);
        g.setColour(HackAudio::Colours::White);
        g.fillPath(p);

        g.setColour(HackAudio::Colours::Black);
        g.strokePath(p, juce::PathStrokeType::PathStrokeType(8));
    }

}

void HackAudio::Button::resized()
{
	if (resizeGuard) { return; }

    int width  = getWidth();
    int height = getHeight();

	resizeGuard = true;

    if (buttonStyle != ButtonStyle::SlidingToggle)
    {

        trackArea.setBounds(0, 0, 0, 0);
        indicatorArea.setBounds(0, 0, 0, 0);
        thumbArea.setBounds(0, 0, 0, 0);

        int fontSize = std::min(width, height) / 4;
        buttonFont.setHeight(fontSize);

        setSize(width, height);

    }
    else
    {

        trackArea.setBounds(32, 16, 64, 32);

        thumbArea.setSize(32, 32);
        indicatorArea.setPosition(32, 16);
        indicatorArea.setHeight(32);

        if (getToggleState())
        {
            thumbArea.setPosition(64, 16);
        }
        else
        {
            thumbArea.setPosition(32, 16);
        }

        indicatorArea.setWidth((thumbArea.getX() - indicatorArea.getX()) + thumbArea.getWidth()/2);

        setSize(128, 64);

    }

	resizeGuard = false;
}
