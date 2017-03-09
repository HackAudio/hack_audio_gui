#include "hack_audio_FlexBox.h"

HackAudio::FlexBox::FlexBox()
{

    justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

}

HackAudio::FlexBox::~FlexBox()
{

}

void HackAudio::FlexBox::addComponent(juce::Component* component)
{
    items.add(juce::FlexItem(*component));
    applyLayout();
}

void HackAudio::FlexBox::removeComponent(juce::Component* component)
{

    for (int i = 0; i < items.size(); ++i)
    {
        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == component || !fi.associatedComponent)
        {
            items.remove(i);
        }
    }
}

void HackAudio::FlexBox::applyBounds(juce::Rectangle<int> bounds)
{
    setBounds(bounds);
    applyLayout();
}

void HackAudio::FlexBox::applyLayout()
{
    performLayout(getBounds());
}
