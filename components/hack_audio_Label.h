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

    bool listenTo(juce::Component* c);

private:

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Component * listeningTarget;

    juce::String prefix;
    juce::String postfix;

    juce::String displayText;

    juce::Rectangle<int> textArea;

    bool isResizing;

};

#endif
