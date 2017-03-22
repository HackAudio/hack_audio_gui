#include "hack_audio_FlexBox.h"

HackAudio::FlexBox::FlexBox()
{

    justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    flexBoxBounds.setBounds(0, 0, 0, 0);

    resizeGuard = false;

}

HackAudio::FlexBox::~FlexBox()
{

}

void HackAudio::FlexBox::addComponent(juce::Component* component)
{
    items.add(juce::FlexItem(*component).withWidth(component->getWidth()).withHeight(component->getHeight()).withAlignSelf(juce::FlexItem::AlignSelf::center));

    component->addComponentListener(this);

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

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow = newFlexGrow;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow, float newFlexShrink)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow   = newFlexGrow;
        items.getReference(i).flexShrink = newFlexShrink;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow, float newFlexShrink, float newFlexBasis)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow   = newFlexGrow;
        items.getReference(i).flexShrink = newFlexShrink;
        items.getReference(i).flexBasis  = newFlexBasis;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemWidth(float newWidth)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).width = newWidth;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemHeight(float newHeight)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).width = newHeight;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemMargin(juce::FlexItem::Margin newMargin)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).margin = newMargin;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyBounds(juce::Rectangle<int> bounds)
{
    flexBoxBounds = bounds;
    applyLayout();
}

void HackAudio::FlexBox::applyLayout()
{

    resizeGuard = true;
    performLayout(flexBoxBounds);
    resizeGuard = false;

}

void HackAudio::FlexBox::componentMovedOrResized(juce::Component &component, bool wasMoved, bool wasResized)
{

    if (resizeGuard) { return; }

    for (int i = 0; i < items.size(); ++i)
    {
        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == &component)
        {
            fi.width = component.getWidth();
        }
    }

    applyLayout();

}
