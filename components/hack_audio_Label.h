#ifndef HACK_AUDIO_LABEL_H
#define HACK_AUDIO_LABEL_H

class Label : public juce::Label
{
public:

    Label();
    ~Label();

    juce::String getPrefix();
    void setPrefix(juce::String pre);

    juce::String getPostfix();
    void setPostfix(juce::String post);

private:

    void paint(juce::Graphics& g) override;

    juce::String prefix;
    juce::String postfix;

    juce::Rectangle<int> textArea;

    bool isResizing;
};

#endif
