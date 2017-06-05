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

#ifndef HACK_AUDIO_NAVIGATIONBUTTON_H
#define HACK_AUDIO_NAVIGATIONBUTTON_H

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace HackAudio
{

/**
 A basic arrow style button to embed in larger components
*/
class NavigationButton : public juce::Button
{
public:

    NavigationButton() : juce::Button("") { setWantsKeyboardFocus(false); }
    ~NavigationButton() {};

    /**
     The direction the button should face
    */
    enum Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    Direction buttonDirection = Left;

private:

    void mouseEnter(const juce::MouseEvent& e) override
    {

        if (isEnabled())
        {

            setMouseCursor(juce::MouseCursor::PointingHandCursor);
            
        }

        juce::Button::mouseEnter(e);

    }

    void mouseExit(const juce::MouseEvent& e) override
    {

        if (isEnabled())
        {

            setMouseCursor(juce::MouseCursor::NormalCursor);

        }

        juce::Button::mouseExit(e);
        
    }

    void enablementChanged() override
    {

        repaint();

    }

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {

        int width  = getWidth();
        int height = getHeight();

        juce::Button::ButtonState state = getState();

        if (state == juce::Button::ButtonState::buttonOver && state != juce::Button::ButtonState::buttonDown)
        {

            g.setColour(HackAudio::Colours::LightGray);

        }
        else if (state == juce::Button::ButtonState::buttonOver || state == juce::Button::ButtonState::buttonDown)
        {

            g.setColour(HackAudio::Colours::DarkGray);

        }
        else
        {

            g.setColour(HackAudio::Colours::White);

        }

        if (!isEnabled())
        {

            g.setColour(HackAudio::Colours::Black);

        }

        if (buttonDirection == Up)
        {

            g.drawLine((width/16), (height - height/4), (width/2), (height/4), 2);
            g.drawLine((width/2), (height/4), (width - width/16), (height - height/4), 2);

        }
        else if (buttonDirection == Down)
        {

            g.drawLine((width/16), (height/4), (width/2), (height - height/4), 2);
            g.drawLine((width/2), (height - height/4), (width - width/16), (height/4), 2);

        }
        else if (buttonDirection == Left)
        {

            g.drawLine((width/4), (height/2), (width - width/4), (height/16), 2);
            g.drawLine((width/4), (height/2), (width - width/4), (height - height/16), 2);

        }
        else if (buttonDirection == Right)
        {

            g.drawLine((width - width/4), (height/2), (width/4), (height/16), 2);
            g.drawLine((width - width/4), (height/2), (width/4), (height - height/16), 2);

        }

    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NavigationButton)

};

}

#endif
