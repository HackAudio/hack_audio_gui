#include "hack_audio_Label.h"

HackAudio::Label::Label()
{

    setInterceptsMouseClicks(true, false);

    setJustificationType(juce::Justification::centred);

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId, HackAudio::Colours::Cyan);

    colourInterpolation.reset(50, 0.5);

    animationStatus = true;
    placeholderStatus = false;
    wasClicked = false;

    timeout = 0;

    addListener(this);

}

HackAudio::Label::~Label()
{

}

juce::String HackAudio::Label::getPrefix()
{

    return prefix;

}

void HackAudio::Label::setPrefix(juce::String preText, juce::NotificationType notificationType)
{

    prefix = preText;

    if (notificationType == juce::sendNotification)
    {
        labelTextChanged(this);
    }

    repaint();

}

juce::String HackAudio::Label::getPostfix()
{

    return postfix;

}

void HackAudio::Label::setPostfix(juce::String postText, juce::NotificationType notificationType)
{

    postfix = postText;

    if (notificationType == juce::sendNotification)
    {
        labelTextChanged(this);
    }

    repaint();

}

void HackAudio::Label::setPlaceholder(juce::String placeholderText)
{
    placeholder = placeholderText;
    placeholderStatus = true;
    repaint();
}

void HackAudio::Label::setPlaceholderStatus(bool shouldShowPlaceholder)
{
    placeholderStatus = shouldShowPlaceholder;
    repaint();
}

juce::String HackAudio::Label::getPlaceholder()
{
    return placeholder;
}

void HackAudio::Label::setAnimationStatus(bool shouldAnimate)
{

    animationStatus = shouldAnimate;

}

void HackAudio::Label::mouseEnter(const juce::MouseEvent& e)
{

    if (placeholderStatus)
    {

        if (findColour(HackAudio::backgroundColourId) == HackAudio::Colours::Gray)
        {

            setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray.withMultipliedBrightness(1.25f));

        }

    }

}

void HackAudio::Label::mouseExit(const juce::MouseEvent& e)
{

    if (placeholderStatus)
    {

        if (findColour(HackAudio::backgroundColourId) == HackAudio::Colours::Gray.withMultipliedBrightness(1.25f))
        {

            setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray);

        }

    }

}

void HackAudio::Label::mouseUp(const juce::MouseEvent& e)
{

    if (placeholderStatus && !e.mouseWasDraggedSinceMouseDown())
    {

        wasClicked = true;
        labelTextChanged(this);

    }
    
}

void HackAudio::Label::labelTextChanged(juce::Label* labelThatHasChanged)
{

    if (animationStatus)
    {

        timeout = 75;
        colourInterpolation.setValue(1.0f);
        startTimerHz(ANIMATION_FPS);

    }

    wasClicked = false;

}

void HackAudio::Label::timerCallback()
{

    if (colourInterpolation.isSmoothing())
    {
        
        repaint();

        if (std::abs(colourInterpolation.getTargetValue() - colourInterpolation.getNextValue()) < 0.0001)
        {
            colourInterpolation.setValue(colourInterpolation.getTargetValue());
        }

    }
    else
    {

        if (colourInterpolation.getTargetValue() == 1.0f)
        {
            colourInterpolation.setValue(0.0f);
        }
        else
        {

            if (timeout > 0)
            {
                timeout--;

            }
            else
            {
                repaint();
                stopTimer();
            }

        }

    }

}

void HackAudio::Label::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillPath(p);

    juce::Colour foreground = findColour(HackAudio::foregroundColourId);
    juce::Colour highlight  = findColour(HackAudio::highlightColourId);

    juce::String textToDisplay;
    textToDisplay = (!isTimerRunning() && placeholderStatus) ? placeholder : prefix + getText() + postfix;

    g.setColour(foreground.interpolatedWith(highlight, colourInterpolation.getNextValue()));

    g.setFont(getFont());
    g.drawFittedText(textToDisplay, 12, 12, width - 24, height - 24, getJustificationType(), 8);

}

void HackAudio::Label::resized()
{

    int width = getWidth();
    int height = getHeight();

    getFont().setHeight(std::min(width, height) / 4);
    
}
