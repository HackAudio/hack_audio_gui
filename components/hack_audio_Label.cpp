#include "hack_audio_Label.h"

HackAudio::Label::Label()
{
    isResizing = false;
}

HackAudio::Label::~Label()
{

}

juce::String HackAudio::Label::getPrefix()
{
    return prefix;
}

void HackAudio::Label::setPrefix(juce::String pre)
{
    prefix = pre;
    repaint();
}

juce::String HackAudio::Label::getPostfix()
{
    return postfix;
}

void HackAudio::Label::setPostfix(juce::String post)
{
    postfix = post;
}

// listenTo

void paint(juce::Graphics& g)
{



}

void resized()
{

    if (isResizing) { return; }

    isResizing = true;



    isResizing = false;

}
