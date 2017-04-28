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

    class Diagram;

public:

    Label();
    ~Label();

    /**
     Sets the prefix that should always display before the label text
     
     @parameter preText     the text to append before the label's main text
     @parameter notificationType    whether to send a change message to any Label::Listener objects
     */
    void setPrefix(juce::String preText, juce::NotificationType notificationType);

    /**
        Returns the label's current prefix string
     */
    juce::String getPrefix();

    /**
     Sets the postfix that should always display after the label text
     
     @parameter postText     the text to append before the label's main text
     @parameter notificationType    whether to send a change message to any Label::Listener objects
    */
    void setPostfix(juce::String postText, juce::NotificationType notificationType);

    /**
     Returns the label's current postfix string
     */
    juce::String getPostfix();

    /**
     Sets the text that displays automatically. When setting new text with animation enabled, the new text will display during the animation and then revert to the placeholder text after the animation completes
     
     @parameter placeholderText the text to show before and after animation
     @parameter notificationType whether to send a change message to any Label::Listener objects
    */
    void setPlaceholder(juce::String placeholderText);

    /**
     Turns the placeholder text feature on or off. This is set to true whenever setPlaceholder() is called
    */
    void setPlaceholderStatus(bool shouldShowPlaceholder);

    /**
     Returns the current placeholder text that the label shows before and after animations. Note that the actual current text is still accessible via getText()
    */
    juce::String getPlaceholder();

    /**
     Toggles whether the label should animate when its text changes
     
     @parameter shouldAnimate   determines whether to animate when setting the text, prefix, or postfix
    */
    void setAnimationStatus(bool shouldAnimate);

    /**
     Returns the current animation status
    */
    bool getAnimationStatus();

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
    bool getFormattingStatus();

private:

    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit (const juce::MouseEvent& e) override;
    void mouseUp   (const juce::MouseEvent& e) override;

    void labelTextChanged(juce::Label* labelThatHasChanged) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    juce::String prefix;
    juce::String postfix;
    juce::String placeholder;

    bool animationStatus;
    bool placeholderStatus;
    bool formattingStatus;

    int timeout;

    juce::LinearSmoothedValue<float> colourInterpolation;

    juce::Rectangle<int> textArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Label)

};

}

#endif
