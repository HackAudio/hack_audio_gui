#include "hack_audio_Label.h"

HackAudio::Label::Label()
{
    setInterceptsMouseClicks(false, false);

    currentColourInterpolation.reset(50, 0.45);

    animationStatus = true;
    animationStatusPrePostFix = false;

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

    if (animationStatusPrePostFix && notificationType == juce::sendNotification)
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

    if (animationStatusPrePostFix && notificationType == juce::sendNotification)
    {
        labelTextChanged(this);
    }

    repaint();
}

void HackAudio::Label::setAnimationStatus(bool shouldAnimate, bool shouldAnimatePrefixPostfix)
{
    animationStatus = shouldAnimate;
    animationStatusPrePostFix = shouldAnimatePrefixPostfix;
}

void HackAudio::Label::labelTextChanged(juce::Label* labelThatHasChanged)
{
    if (animationStatus || animationStatusPrePostFix)
    {
        currentColourInterpolation.setValue(1.0f);
        startTimerHz(60);
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
            if (currentColourInterpolation.getNextValue() == 0.0f)
            {
                stopTimer();
            }
            return;
        }
    }
}

void HackAudio::Label::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
    g.setColour(HackAudio::Colours::Black);
    g.fillPath(p);

    g.setColour(HackAudio::Colours::White.interpolatedWith(HackAudio::Colours::Cyan, currentColourInterpolation.getNextValue()));

    g.setFont(getFont());
    g.drawText(prefix + getText() + postfix, 0, 0, width, height, juce::Justification::centred, 1);

}

void HackAudio::Label::resized()
{

    int width = getWidth();
    int height = getHeight();

    getFont().setHeight(std::min(width, height) / 4);
    
}
