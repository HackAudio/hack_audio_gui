#ifndef HACK_AUDIO_LABEL_H
#define HACK_AUDIO_LABEL_H

namespace HackAudio
{

/**

 A custom label using the generic juce::Slider API that supports pre/post-fixes 
 
 */

class Label : public juce::Label,
              public juce::LabelListener,
              public juce::Timer
{
public:

    Label();
    ~Label();

    /**
     Sets the prefix that should always display before the label text
     */
    void setPrefix(juce::String pre);

    /**
        Returns the label's current prefix string
     */
    juce::String getPrefix();


    /**
        Sets the postfix that should always display after the label text
    */
    void setPostfix(juce::String post);

    /**
     Returns the label's current postfix string
     */
    juce::String getPostfix();

    /**
     Toggles whether the label should animate when its text changes
     
     @parameter shouldAnimate               determines whether to animate on setText()
     @parameter shouldAnimatePrePostfix  determines whether to animate when setPrefix() or setPostfix() are called
    */
    void setAnimationStatus(bool shouldAnimate, bool shouldAnimatePrePostfix=false);

private:

    void labelTextChanged(juce::Label* labelThatHasChanged) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    juce::String prefix;
    juce::String postfix;

    bool animationStatus;
    bool animationStatusPrePostFix;

    juce::LinearSmoothedValue<float> currentColourInterpolation;

    juce::Rectangle<int> textArea;

};

}

#endif
