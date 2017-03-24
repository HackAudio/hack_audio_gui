#ifndef HACK_AUDIO_LABEL_H
#define HACK_AUDIO_LABEL_H

namespace HackAudio
{

/**

 A custom label using the generic juce::Slider API that supports pre/post-fixes 
 
 */

class Label : public juce::Label,
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
     Toggles whether the label should animate when its text changes
     
     @parameter shouldAnimate   determines whether to animate when setting the text, prefix, or postfix
    */
    void setAnimationStatus(bool shouldAnimate);

private:

    void labelTextChanged(juce::Label* labelThatHasChanged) override;

    void parentHierarchyChanged() override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    juce::String prefix;
    juce::String postfix;

    bool animationStatus;

    juce::LinearSmoothedValue<float> currentColourInterpolation;

    juce::Rectangle<int> textArea;

};

}

#endif
