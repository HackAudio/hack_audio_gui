#include "hack_audio_Label.h"

HackAudio::Label::Label()
{
    setInterceptsMouseClicks(false, false);
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
    repaint();
}

void HackAudio::Label::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
    g.setColour(HackAudio::Colours::Black);
    g.fillPath(p);

    g.setColour(HackAudio::Colours::White);
    g.setFont(getFont());
    g.drawText(prefix + getText() + postfix, 0, 0, width, height, juce::Justification::centred, 1);

}

void HackAudio::Label::resized()
{

    int width = getWidth();
    int height = getHeight();

    setFont(juce::Font(std::min(width, height) / 4));
    
}
