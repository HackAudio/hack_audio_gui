#include <regex>

#include "../utils/hack_audio_TextFormatting.h"

HackAudio::Label::Label()
{

    setInterceptsMouseClicks(true, false);

    setFont(HackAudio::Fonts::Now.withHeight(DEFAULT_FONTSIZE));

    setJustificationType(juce::Justification::centred);

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId, HackAudio::Colours::Cyan);

    colourInterpolation.reset(50, 0.5);

    animationStatus = true;
    placeholderStatus = false;
    formattingStatus = true;

    timeout = 0;

    addListener(this);

}

HackAudio::Label::~Label()
{

}

void HackAudio::Label::setText(const char* newText, juce::NotificationType notification)
{
    juce::String charPtr = juce::String::CharPointerType::CharPointer_UTF8(newText);
    juce::Label::setText(charPtr, notification);
}

void HackAudio::Label::setText(const juce::String newText, juce::NotificationType notification)
{
    juce::Label::setText(newText, notification);
}

juce::String HackAudio::Label::getPrefix()
{

    return prefix;

}

void HackAudio::Label::setPrefix(const char* preText, juce::NotificationType notificationType)
{

    prefix = juce::String::CharPointerType::CharPointer_UTF8(preText);

    if (notificationType != juce::dontSendNotification)
    {
        labelTextChanged(this);
        callChangeListeners();
    }

    repaint();

}

void HackAudio::Label::setPrefix(const juce::String preText, juce::NotificationType notificationType)
{

    prefix = preText;

    if (notificationType != juce::dontSendNotification)
    {
        labelTextChanged(this);
        callChangeListeners();
    }

    repaint();

}

juce::String HackAudio::Label::getPostfix()
{

    return postfix;

}

void HackAudio::Label::setPostfix(const char* postText, juce::NotificationType notificationType)
{

    postfix = juce::String::CharPointerType::CharPointer_UTF8(postText);

    if (notificationType != juce::dontSendNotification)
    {
        labelTextChanged(this);
        callChangeListeners();
    }

    repaint();

}

void HackAudio::Label::setPostfix(const juce::String postText, juce::NotificationType notificationType)
{

    postfix = postText;

    if (notificationType != juce::dontSendNotification)
    {
        labelTextChanged(this);
        callChangeListeners();
    }

    repaint();

}

void HackAudio::Label::setPlaceholder(const char* placeholderText)
{

    placeholder = juce::String::CharPointerType::CharPointer_UTF8(placeholderText);
    placeholderStatus = true;
    repaint();
    
}

void HackAudio::Label::setPlaceholder(const juce::String placeholderText)
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

bool HackAudio::Label::getAnimationStatus()
{

    return animationStatus;

}

void HackAudio::Label::setFormattingStatus(bool shouldFormatText)
{

    formattingStatus = shouldFormatText;
    repaint();

}

bool HackAudio::Label::getFormattingStatus()
{

    return formattingStatus;

}

void HackAudio::Label::mouseEnter(const juce::MouseEvent& e)
{

    if (placeholderStatus && getText().isNotEmpty())
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray.withMultipliedBrightness(1.25f));

    }

}

void HackAudio::Label::mouseExit(const juce::MouseEvent& e)
{

    if (findColour(HackAudio::backgroundColourId) != HackAudio::Colours::Gray)
    {
        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray);
    }

}

void HackAudio::Label::mouseUp(const juce::MouseEvent& e)
{

    if (placeholderStatus && !e.mouseWasDraggedSinceMouseDown())
    {

        if (getText().isNotEmpty())
        {

            labelTextChanged(this);

        }

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
    p.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillPath(p);

    juce::Colour foreground = findColour(HackAudio::foregroundColourId);
    juce::Colour highlight  = findColour(HackAudio::highlightColourId);

    g.setColour(foreground.interpolatedWith(highlight, colourInterpolation.getNextValue()));


    if (formattingStatus)
    {


        if (!isTimerRunning() && placeholderStatus)
        {
            if (placeholder.containsAnyOf("^_") || placeholder.contains("\\array"))
            {
                formatText(placeholder, getFont(), getJustificationType(), CORNER_RADIUS, CORNER_RADIUS, width - (CORNER_RADIUS * 2), height - (CORNER_RADIUS * 2)).draw(g);
                return;
            }
        }
        else
        {
            if (getText().containsAnyOf("^_") || getText().contains("\\array"))
            {
                formatText(getText(), getFont(), getJustificationType(), CORNER_RADIUS, CORNER_RADIUS, width - (CORNER_RADIUS * 2), height - (CORNER_RADIUS * 2)).draw(g);
                return;
            }
        }

    }

    g.setFont(getFont());

    juce::String textToDisplay;
    textToDisplay = (!isTimerRunning() && placeholderStatus) ? placeholder : prefix + getText() + postfix;

    g.drawFittedText(textToDisplay, CORNER_RADIUS / 2, CORNER_RADIUS / 2, width - CORNER_RADIUS, height - CORNER_RADIUS, getJustificationType(), 1, 1.0f);

}
