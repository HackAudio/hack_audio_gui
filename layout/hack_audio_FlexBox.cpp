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

void HackAudio::FlexBox::setDirection(juce::FlexBox::Direction newDirection)
{
    flexDirection = newDirection;
    applyLayout();
}

void HackAudio::FlexBox::setWrap(juce::FlexBox::Wrap newWrap)
{
    flexWrap = newWrap;
    applyLayout();
}

void HackAudio::FlexBox::setAlignContent(juce::FlexBox::AlignContent newAlignment)
{
    alignContent = newAlignment;
    applyLayout();
}

void HackAudio::FlexBox::setAlignItems(juce::FlexBox::AlignItems newAlignment)
{
    alignItems = newAlignment;
    applyLayout();
}

void HackAudio::FlexBox::setJustifyContent(juce::FlexBox::JustifyContent newJustification)
{
    justifyContent = newJustification;
    applyLayout();
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

void HackAudio::FlexBox::applyGlobalFlexItem(juce::FlexItem newFlexProperties)
{
    for (int i = 0; i < items.size(); ++i)
    {
        juce::Component * c = items[i].associatedComponent;
        items[i] = newFlexProperties;
        items.getReference(i).associatedComponent = c;
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
            fi.width  = component.getWidth();
            fi.height = component.getHeight();
        }
    }

    applyLayout();

}

void HackAudio::FlexBox::componentVisibilityChanged(juce::Component &component)
{

    for (int i = 0; i < items.size(); ++i)
    {
        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == &component)
        {
            if (component.isVisible())
            {
                fi.width = component.getProperties().getWithDefault(juce::Identifier("flexWidthCache"), juce::var(0));
                fi.height = component.getProperties().getWithDefault(juce::Identifier("flexHeightCache"), juce::var(0));
                component.getProperties().remove(juce::Identifier("flexWidthCache"));
                component.getProperties().remove(juce::Identifier("flexHeightCache"));
            }
            else
            {
                component.getProperties().set(juce::Identifier("flexWidthCache"), juce::var(component.getWidth()));
                component.getProperties().set(juce::Identifier("flexHeightCache"), juce::var(component.getHeight()));
                fi.width  = 0;
                fi.height = 0;
            }
        }
    }

    applyLayout();

}
