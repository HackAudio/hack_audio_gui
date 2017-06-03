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

#include <regex>

#include "../utils/hack_audio_TextFormatting.h"

HackAudio::Label::Label()
{

    setInterceptsMouseClicks(true, false);

    setFont(HackAudio::Fonts::NowRegular.withHeight(HackAudio::FontHeights::Medium));

    setJustificationType(juce::Justification::centred);

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId, HackAudio::Colours::Cyan);

    colourInterpolation.reset(50, 0.5);
    backgroundInterpolation.reset(50, 0.35);

    animationStatus   = true;
    placeholderStatus = false;
    formattingStatus  = true;
    highlightStatus   = false;

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

juce::String HackAudio::Label::getPrefix() const
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

juce::String HackAudio::Label::getPostfix() const
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

juce::String HackAudio::Label::getPlaceholder() const
{
    return placeholder;
}

void HackAudio::Label::setAnimationStatus(bool shouldAnimate)
{

    animationStatus = shouldAnimate;

}

bool HackAudio::Label::getAnimationStatus() const
{

    return animationStatus;

}

void HackAudio::Label::setFormattingStatus(bool shouldFormatText)
{

    formattingStatus = shouldFormatText;
    repaint();

}

bool HackAudio::Label::getFormattingStatus() const
{

    return formattingStatus;

}

void HackAudio::Label::mouseEnter(const juce::MouseEvent& e)
{

    if (!isEnabled()) { return; }

    if ((placeholderStatus && getText().isNotEmpty()) || highlightStatus)
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::LightGray);
        setMouseCursor(juce::MouseCursor::PointingHandCursor);

    }

}

void HackAudio::Label::mouseExit(const juce::MouseEvent& e)
{

    if (findColour(HackAudio::backgroundColourId) != HackAudio::Colours::Gray)
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Gray);
        setMouseCursor(juce::MouseCursor::NormalCursor);

    }

}

void HackAudio::Label::mouseDown(const juce::MouseEvent& e)
{

    if (!isEnabled()) { return; }

    if ((placeholderStatus && getText().isNotEmpty()) || highlightStatus)
    {

        backgroundInterpolation.setValue(1.0f);
        startTimer(backgroundAnimation, 1000.0f / (float)ANIMATION_FPS);

    }

}

void HackAudio::Label::mouseUp(const juce::MouseEvent& e)
{

    if (!isEnabled()) { return; }

    if (findColour(HackAudio::backgroundColourId) == HackAudio::Colours::DarkGray)
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::LightGray);
        
    }

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
        startTimer(foregroundAnimation, 1000.0f / (float)ANIMATION_FPS);

    }

}

void HackAudio::Label::timerCallback(int timerID)
{

    if (timerID == foregroundAnimation)
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
                    stopTimer(foregroundAnimation);
                }

            }

        }

    }
    else
    {

        if (backgroundInterpolation.isSmoothing())
        {

            repaint();

            if (std::abs(backgroundInterpolation.getTargetValue() - backgroundInterpolation.getNextValue()) < 0.0001)
            {

                backgroundInterpolation.setValue(backgroundInterpolation.getTargetValue());

            }

        }
        else
        {

            if (backgroundInterpolation.getTargetValue() == 1.0f && !juce::Component::isMouseButtonDownAnywhere())
            {

                backgroundInterpolation.setValue(0.0f);

            }
            else if (backgroundInterpolation.getTargetValue() == 1.0f)
            {

                backgroundInterpolation.setValue(1.0f);

            }
            else
            {

                if (backgroundInterpolation.getNextValue() == 0.0f)
                {

                    backgroundInterpolation.setValue(0.0f);
                    stopTimer(backgroundAnimation);

                }

                return;

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
    juce::Colour background = findColour(HackAudio::backgroundColourId).interpolatedWith(HackAudio::Colours::DarkGray, backgroundInterpolation.getNextValue());
    g.setColour(background);
    g.fillPath(p);

    juce::Colour foreground = findColour(HackAudio::foregroundColourId);
    juce::Colour highlight  = findColour(HackAudio::highlightColourId);

    g.setColour(foreground.interpolatedWith(highlight, colourInterpolation.getNextValue()));

    if (formattingStatus)
    {

        if (!isTimerRunning(foregroundAnimation) && placeholderStatus)
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
    textToDisplay = (!isTimerRunning(foregroundAnimation) && placeholderStatus) ? placeholder : prefix + getText() + postfix;

    g.drawFittedText(textToDisplay, CORNER_RADIUS / 2, CORNER_RADIUS / 2, width - CORNER_RADIUS, height - CORNER_RADIUS, getJustificationType(), 1, 1.0f);

}
