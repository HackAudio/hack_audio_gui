#include "hack_audio_Label.h"

HackAudio::Label::Label()
{

    setInterceptsMouseClicks(false, false);

    setJustificationType(juce::Justification::centred);

    setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::ColourIds::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::ColourIds::highlightColourId, HackAudio::Colours::Cyan);

    currentColourInterpolation.reset(50, 0.5);

    animationStatus = true;
    placeholderStatus = false;

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

void HackAudio::Label::labelTextChanged(juce::Label* labelThatHasChanged)
{

    if (animationStatus)
    {
        timeout = 75;
        currentColourInterpolation.setValue(1.0f);
        startTimerHz(ANIMATION_FPS);
    }

}

void HackAudio::Label::timerCallback()
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

        if (currentColourInterpolation.getTargetValue() == 1.0f)
        {
            currentColourInterpolation.setValue(0.0f);
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
    g.setColour(findColour(HackAudio::ColourIds::backgroundColourId));
    g.fillPath(p);

    juce::Colour foreground = findColour(HackAudio::ColourIds::foregroundColourId);
    juce::Colour highlight  = findColour(HackAudio::ColourIds::highlightColourId);

    juce::String textToDisplay;
    textToDisplay = (!isTimerRunning() && placeholderStatus) ? placeholder : prefix + getText() + postfix;

    g.setColour(foreground.interpolatedWith(highlight, currentColourInterpolation.getNextValue()));

    g.setFont(getFont());
    g.drawFittedText(textToDisplay, 12, 12, width - 24, height - 24, getJustificationType(), 8);

}

void HackAudio::Label::resized()
{

    int width = getWidth();
    int height = getHeight();

    getFont().setHeight(std::min(width, height) / 4);
    
}
