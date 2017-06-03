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

#ifndef HACK_AUDIO_LABEL_H
#define HACK_AUDIO_LABEL_H

namespace HackAudio
{

/**

 A custom label using the generic juce::Slider API that supports pre/post-fixes 
 
 */

class Label : public  juce::Label,
              private juce::LabelListener,
              private juce::Timer
{

    friend class Diagram;

public:

    Label();
    ~Label();

    /** 
        A wrapper for juce::Label::setText allowing for non-UTF8 characters
     
        CAVEAT: Will not work on label pointers, pointers will call base class function instead
     */
    void setText(const char* newText, juce::NotificationType notification);
    void setText(const juce::String newText, juce::NotificationType notification);

    /**
     Sets the prefix that should always display before the label text
     
     @param preText     the text to append before the label's main text
     @param notificationType    whether to send a change message to any Label::Listener objects
     */
    void setPrefix(const char* preText, juce::NotificationType notificationType);
    void setPrefix(const juce::String preText, juce::NotificationType notificationType);

    /**
        Returns the label's current prefix string
     */
    juce::String getPrefix() const;

    /**
     Sets the postfix that should always display after the label text
     
     @param postText     the text to append before the label's main text
     @param notificationType    whether to send a change message to any Label::Listener objects
    */
    void setPostfix(const char* postText, juce::NotificationType notificationType);
    void setPostfix(const juce::String postText, juce::NotificationType notificationType);

    /**
     Returns the label's current postfix string
     */
    juce::String getPostfix() const;

    /**
     Sets the text that displays automatically. When setting new text with animation enabled, the new text will display during the animation and then revert to the placeholder text after the animation completes
     
     @param placeholderText the text to show before and after animation
     @param notificationType whether to send a change message to any Label::Listener objects
    */
    void setPlaceholder(const char* placeholderText);
    void setPlaceholder(const juce::String placeholderText);

    /**
     Turns the placeholder text feature on or off. This is set to true whenever setPlaceholder() is called
    */
    void setPlaceholderStatus(bool shouldShowPlaceholder);

    /**
     Returns the current placeholder text that the label shows before and after animations. Note that the actual current text is still accessible via getText()
    */
    juce::String getPlaceholder() const;

    /**
     Toggles whether the label should animate when its text changes
     
     @param shouldAnimate   determines whether to animate when setting the text, prefix, or postfix
    */
    void setAnimationStatus(bool shouldAnimate);

    /**
     Returns the current animation status
    */
    bool getAnimationStatus() const;

    /**
     Toggles whether or not the label should format its text (both placeholder and label text)
     
     The text formatting currently supports LaTeX style super and sub script (^ and _) with extended script using brackets (e.g. a^{bcd})
     It also supports square matrix displays using the syntax: 
     
        \\array 1 & 2 & 3 &// 4 & 5 & 6 \\end
     
     Where & denotes column delimiters and &// represents newlines

    */
    void setFormattingStatus(bool shouldFormatText);

    /**
     Returns the current formatting status
    */
    bool getFormattingStatus() const;

private:

    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit (const juce::MouseEvent& e) override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseUp   (const juce::MouseEvent& e) override;

    void labelTextChanged(juce::Label* labelThatHasChanged) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;

    juce::String prefix;
    juce::String postfix;
    juce::String placeholder;

    bool animationStatus;
    bool placeholderStatus;
    bool formattingStatus;
    bool highlightStatus;

    int timeout;

    juce::LinearSmoothedValue<float> colourInterpolation;

    juce::Rectangle<int> textArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Label)

};

}

#endif
