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

#ifndef HACK_AUDIO_BUTTON_H
#define HACK_AUDIO_BUTTON_H

namespace HackAudio
{

/**

 A custom button using the generic juce::Slider API while adding custom styling and new methods.
 
 */

class Button : public juce::Button,
               private juce::Timer
{
public:

    Button();
    ~Button();

    /**
     The available HackAudio::Button styles
     
     BarSingleton is a square button that saves no state and simply sends a singleton event
     
     BarToggle saves its state and colors itself accordingly
     
     SlidingToggle saves state and aesthetically emulates a toggle switch
    */
    enum ButtonStyle
    {
        BarSingleton,
        BarToggle,
        SlidingToggle
    };

    /**
     A char* wrapper for setButtonText()
     */
    void setButtonText(const char* newText);

    /**
     Sets the button's current style
     
     @see ButtonStyle
    */
    void setButtonStyle(ButtonStyle style);

    /**
     Return the button's current style
     
     @see ButtonStyle
    */
    ButtonStyle getButtonStyle() const;

    /**
     Sets the button's font style
    */
    void setFont(juce::Font font);

    /**
     Return the button's current font
    */
    juce::Font getFont() const;

private:

    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp  (const juce::MouseEvent& e) override;

    bool keyPressed(const juce::KeyPress& key) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost  (juce::Component::FocusChangeType cause) override;

    void enablementChanged() override;

    void timerCallback() override;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    void resized() override;

	float animationAcc;
	float animationVel;
	juce::Point<int> animationStart;
	juce::Point<int> animationEnd;

    juce::LinearSmoothedValue<float> colourInterpolation;

	bool resizeGuard;
    juce::Rectangle<int> thumbArea;
	juce::Rectangle<int> trackArea;
	juce::Rectangle<int> indicatorArea;

    bool isDraggable;

    juce::Font buttonFont;

    ButtonStyle buttonStyle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)

};

}

#endif
