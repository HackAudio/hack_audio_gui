#include "hack_audio_FlexBox.h"

HackAudio::FlexBox::FlexBox()
{

    flexDirection  = juce::FlexBox::Direction::row;
    alignItems     = juce::FlexBox::AlignItems::center;
    alignContent   = juce::FlexBox::AlignContent::center;
    justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    flexBoxBounds.setBounds(0, 0, 0, 0);

    defaultFlexSettings.alignSelf = juce::FlexItem::AlignSelf::center;

    resizeGuard = false;

    validBounds = false;

}

HackAudio::FlexBox::~FlexBox()
{

}

void HackAudio::FlexBox::setDefaultItem(juce::FlexItem item, bool updateExistingItems)
{

    defaultFlexSettings = item;

    if (updateExistingItems)
    {
        applyGlobalFlexItem(item);
    }

}

void HackAudio::FlexBox::addComponent(juce::Component& component, int customOrder)
{

    juce::FlexItem itemToAdd = defaultFlexSettings;
    itemToAdd.associatedComponent = &component;
    itemToAdd.width  = component.getWidth();
    itemToAdd.height = component.getHeight();
    itemToAdd.order  = customOrder;
    items.add(itemToAdd);

    component.addComponentListener(this);

    applyLayout();

}

void HackAudio::FlexBox::removeComponent(juce::Component& component)
{
    
    for (int i = 0; i < items.size(); ++i)
    {

        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == &component)
        {
            items.remove(i);
        }
        
    }

    applyLayout();

}

void HackAudio::FlexBox::addFlexBox(juce::FlexBox& flexbox, int customOrder)
{

    juce::FlexItem itemToAdd = defaultFlexSettings;
    itemToAdd.associatedFlexBox = &flexbox;
    itemToAdd.order = customOrder;
    items.add(itemToAdd);

    applyLayout();
    
}

void HackAudio::FlexBox::addFlexBox(HackAudio::FlexBox& flexbox, int customOrder)
{
    juce::FlexItem itemToAdd = defaultFlexSettings;
    itemToAdd.associatedFlexBox = &flexbox;
    itemToAdd.order = customOrder;
    items.add(itemToAdd);

    applyLayout();
}

void HackAudio::FlexBox::removeFlexBox(juce::FlexBox& flexbox)
{

    for (int i = 0; i < items.size(); ++i)
    {

        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedFlexBox == &flexbox)
        {
            items.remove(i);
        }

    }

    applyLayout();

}

void HackAudio::FlexBox::removeFlexBox(HackAudio::FlexBox& flexbox)
{

    for (int i = 0; i < items.size(); ++i)
    {

        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedFlexBox == &flexbox)
        {
            items.remove(i);
        }
        
    }

    applyLayout();

}

void HackAudio::FlexBox::setDirection(juce::FlexBox::Direction newDirection)
{
    flexDirection = newDirection;
    applyLayout();
}

juce::FlexBox::Direction HackAudio::FlexBox::getDirection()
{
    return flexDirection;
}

void HackAudio::FlexBox::setWrap(juce::FlexBox::Wrap newWrap)
{
    flexWrap = newWrap;
    applyLayout();
}

juce::FlexBox::Wrap HackAudio::FlexBox::getWrap()
{
    return flexWrap;
}

void HackAudio::FlexBox::setAlignContent(juce::FlexBox::AlignContent newAlignment)
{
    alignContent = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignContent HackAudio::FlexBox::getAlignContent()
{
    return alignContent;
}

void HackAudio::FlexBox::setAlignItems(juce::FlexBox::AlignItems newAlignment)
{
    alignItems = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignItems HackAudio::FlexBox::getAlignItems()
{
    return alignItems;
}

void HackAudio::FlexBox::setJustifyContent(juce::FlexBox::JustifyContent newJustification)
{
    justifyContent = newJustification;
    applyLayout();
}

juce::FlexBox::JustifyContent HackAudio::FlexBox::getJustifyContent()
{
    return justifyContent;
}

juce::Array<juce::FlexItem> HackAudio::FlexBox::getItems()
{
    return items;
}

juce::FlexItem* HackAudio::FlexBox::getItem(juce::Component& component)
{

    for (int i = 0; i < items.size(); ++i)
    {

        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == &component)
        {

            return &fi;

        }
    }

    return nullptr;

}

juce::FlexItem* HackAudio::FlexBox::getItem(juce::FlexBox& flexbox)
{

    for (int i = 0; i < items.size(); ++i)
    {

        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedFlexBox == &flexbox)
        {

            return &fi;

        }
    }
    
    return nullptr;

}

void HackAudio::FlexBox::setItem(juce::Component& component, juce::FlexItem newFlexProperties)
{

    for (int i = 0; i < items.size(); ++i)
    {

        if (items[i].associatedComponent == &component)
        {

            newFlexProperties.associatedComponent = &component;
            newFlexProperties.width  = component.getWidth();
            newFlexProperties.height = component.getHeight();

            items[i] = newFlexProperties;
            
        }
    }

}

void HackAudio::FlexBox::setItem(juce::FlexBox& flexbox, juce::FlexItem newFlexProperties)
{

    for (int i = 0; i < items.size(); ++i)
    {

        if (items[i].associatedFlexBox == &flexbox)
        {
            newFlexProperties.associatedFlexBox = &flexbox;
            items[i] = newFlexProperties;
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
    validBounds = true;
    applyLayout();

}

void HackAudio::FlexBox::applyLayout()
{

    if (!validBounds) { return; }

    for (int i = 0; i < items.size(); ++i)
    {
        assert(items[i].associatedComponent || items[i].associatedFlexBox);
    }

    resizeGuard = true;
    performLayout(flexBoxBounds);
    resizeGuard = false;

}

void HackAudio::FlexBox::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{

    if (resizeGuard) { return; }

    if (component.getWidth() == 0 || component.getHeight() == 0)
    {

        for (int i = 0; i < items.size(); ++i)
        {

            juce::FlexItem& fi = items.getReference(i);

            if (fi.associatedComponent == &component)
            {

                component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(i));

                items.remove(i);

            }
        }
    }
    else
    {
        if(component.getProperties().contains(juce::Identifier("flexIndexCache")))
        {

            juce::FlexItem fi = defaultFlexSettings;

            fi.associatedComponent = &component;

            fi.width  = component.getWidth();
            fi.height = component.getHeight();

            int index = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
            items.insert(index, fi);

            component.getProperties().remove(juce::Identifier("flexIndexCache"));

        }
        else
        {
            for (int i = 0; i < items.size(); ++i)
            {
                juce::FlexItem& fi = items.getReference(i);

                if (fi.associatedComponent == &component)
                {

                    fi.width  = component.getWidth();
                    fi.height = component.getHeight();

                }
            }
        }
    }

    applyLayout();

}

void HackAudio::FlexBox::componentVisibilityChanged(juce::Component& component)
{

    if (component.isVisible())
    {

        juce::FlexItem fi = defaultFlexSettings;

        fi.associatedComponent = &component;
        fi.width = component.getProperties().getWithDefault(juce::Identifier("flexWidthCache"), juce::var(0));
        fi.height = component.getProperties().getWithDefault(juce::Identifier("flexHeightCache"), juce::var(0));

        int index = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
        items.insert(index, fi);

        component.getProperties().remove(juce::Identifier("flexWidthCache"));
        component.getProperties().remove(juce::Identifier("flexHeightCache"));
        component.getProperties().remove(juce::Identifier("flexIndexCache"));

    }
    else
    {
        for (int i = 0; i < items.size(); ++i)
        {

            juce::FlexItem& fi = items.getReference(i);

            if (fi.associatedComponent == &component)
            {

                component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(i));
                component.getProperties().set(juce::Identifier("flexWidthCache"), juce::var(component.getWidth()));
                component.getProperties().set(juce::Identifier("flexHeightCache"), juce::var(component.getHeight()));

                items.remove(i);

            }
        }
    }

    applyLayout();

}
