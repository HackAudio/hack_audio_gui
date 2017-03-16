#ifndef HACK_AUDIO_LABEL_H
#define HACK_AUDIO_LABEL_H

namespace HackAudio
{

/**

 A custom label using the generic juce::Slider API that supports pre/post-fixes 
 
 */

class Label : public juce::Label
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

private:

    void paint(juce::Graphics& g) override;

    void resized() override;

    juce::String prefix;
    juce::String postfix;

    juce::Rectangle<int> textArea;

    bool resizeGuard;
};

}

#endif
