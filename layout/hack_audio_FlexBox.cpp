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
    
    juce::FlexItem* fi = getItem(component);
    items.remove(fi);

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

void HackAudio::FlexBox::removeFlexBox(juce::FlexBox& flexbox)
{

    juce::FlexItem* fi = getItem(flexbox);
    items.remove(fi);

    applyLayout();

}

void HackAudio::FlexBox::setDirection(juce::FlexBox::Direction newDirection)
{
    flexDirection = newDirection;
    applyLayout();
}

juce::FlexBox::Direction HackAudio::FlexBox::getDirection() const
{
    return flexDirection;
}

void HackAudio::FlexBox::setWrap(juce::FlexBox::Wrap newWrap)
{
    flexWrap = newWrap;
    applyLayout();
}

juce::FlexBox::Wrap HackAudio::FlexBox::getWrap() const
{
    return flexWrap;
}

void HackAudio::FlexBox::setAlignContent(juce::FlexBox::AlignContent newAlignment)
{
    alignContent = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignContent HackAudio::FlexBox::getAlignContent() const
{
    return alignContent;
}

void HackAudio::FlexBox::setAlignItems(juce::FlexBox::AlignItems newAlignment)
{
    alignItems = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignItems HackAudio::FlexBox::getAlignItems() const
{
    return alignItems;
}

void HackAudio::FlexBox::setJustifyContent(juce::FlexBox::JustifyContent newJustification)
{
    justifyContent = newJustification;
    applyLayout();
}

juce::FlexBox::JustifyContent HackAudio::FlexBox::getJustifyContent() const
{
    return justifyContent;
}

juce::Array<juce::FlexItem> HackAudio::FlexBox::getItems() const
{
    return items;
}

juce::FlexItem* HackAudio::FlexBox::getItem(juce::Component& component) const
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

juce::FlexItem* HackAudio::FlexBox::getItem(juce::FlexBox& flexbox) const
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

    juce::FlexItem* fi = getItem(component);

    newFlexProperties.associatedComponent = &component;
    newFlexProperties.width  = component.getWidth();
    newFlexProperties.height = component.getHeight();

    *fi = newFlexProperties;

}

void HackAudio::FlexBox::setItem(juce::FlexBox& flexbox, juce::FlexItem newFlexProperties)
{

    juce::FlexItem* fi = getItem(flexbox);

    newFlexProperties.associatedFlexBox = &flexbox;

    *fi = newFlexProperties;

}

void HackAudio::FlexBox::setItemFlex(juce::Component &component, float newFlexGrow)
{

    juce::FlexItem* fi = getItem(component);
    fi->flexGrow   = newFlexGrow;

    applyLayout();

}

void HackAudio::FlexBox::setItemFlex(juce::Component &component, float newFlexGrow, float newFlexShrink)
{

    juce::FlexItem* fi = getItem(component);
    fi->flexGrow   = newFlexGrow;
    fi->flexShrink = newFlexShrink;

    applyLayout();
    
}

void HackAudio::FlexBox::setItemFlex(juce::Component &component, float newFlexGrow, float newFlexShrink, float newFlexBasis)
{

    juce::FlexItem* fi = getItem(component);
    fi->flexGrow   = newFlexGrow;
    fi->flexShrink = newFlexShrink;
    fi->flexBasis  = newFlexBasis;

    applyLayout();
    
}

void HackAudio::FlexBox::setItemFlex(juce::FlexBox& flexbox, float newFlexGrow)
{

    juce::FlexItem* fi = getItem(flexbox);
    fi->flexGrow   = newFlexGrow;

    applyLayout();

}

void HackAudio::FlexBox::setItemFlex(juce::FlexBox& flexbox, float newFlexGrow, float newFlexShrink)
{

    juce::FlexItem* fi = getItem(flexbox);
    fi->flexGrow   = newFlexGrow;
    fi->flexShrink = newFlexShrink;

    applyLayout();

}

void HackAudio::FlexBox::setItemFlex(juce::FlexBox& flexbox, float newFlexGrow, float newFlexShrink, float newFlexBasis)
{

    juce::FlexItem* fi = getItem(flexbox);
    fi->flexGrow   = newFlexGrow;
    fi->flexShrink = newFlexShrink;
    fi->flexBasis  = newFlexBasis;

    applyLayout();
    
}

void HackAudio::FlexBox::setItemWidth(juce::Component &component, float newWidth)
{

    juce::FlexItem* fi = getItem(component);
    fi->width = newWidth;

    applyLayout();

}

void HackAudio::FlexBox::setItemWidth(juce::FlexBox &flexbox, float newWidth)
{

    juce::FlexItem* fi = getItem(flexbox);
    fi->width = newWidth;

    applyLayout();

}

void HackAudio::FlexBox::setItemHeight(juce::Component& component, float newHeight)
{

    juce::FlexItem* fi = getItem(component);
    fi->height = newHeight;

    applyLayout();

}

void HackAudio::FlexBox::setItemHeight(juce::FlexBox &flexbox, float newHeight)
{

    juce::FlexItem* fi = getItem(flexbox);
    fi->height = newHeight;

    applyLayout();

}

void HackAudio::FlexBox::setItemMargin(juce::Component &component, juce::FlexItem::Margin newMargin)
{

    juce::FlexItem* fi = getItem(component);
    fi->margin = newMargin;

    applyLayout();

}

void HackAudio::FlexBox::setItemMargin(juce::FlexBox &flexbox, juce::FlexItem::Margin newMargin)
{
    
    juce::FlexItem* fi = getItem(flexbox);
    fi->margin = newMargin;

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
    validBounds = true;

    applyLayout();

}

void HackAudio::FlexBox::applyBounds(juce::Rectangle<float> bounds)
{

    juce::Rectangle<int> r(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    flexBoxBounds = r;
    validBounds = true;

    applyLayout();

}

void HackAudio::FlexBox::applyBounds(int x, int y, int width, int height)
{

    juce::Rectangle<int> r(x, y, width, height);
    flexBoxBounds = r;
    validBounds = true;

    applyLayout();

}

void HackAudio::FlexBox::applyLayout()
{

    if (!validBounds) { return; }

    for (int i = 0; i < items.size(); ++i)
    {
        assert(items[i].associatedComponent || items[i].associatedFlexBox); /* Warning: FlexItem Does Not Control Anything */
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

        juce::FlexItem* fi = getItem(component);

        component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(fi->order));

        items.remove(fi);

    }
    else
    {
        if(component.getProperties().contains(juce::Identifier("flexIndexCache")))
        {

            juce::FlexItem fi = defaultFlexSettings;

            fi.associatedComponent = &component;

            fi.width  = component.getWidth();
            fi.height = component.getHeight();

            fi.order = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
            items.add(fi);

            component.getProperties().remove(juce::Identifier("flexIndexCache"));

        }
        else
        {

            juce::FlexItem* fi = getItem(component);

            fi->width  = component.getWidth();
            fi->height = component.getHeight();

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

        fi.order = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
        items.add(fi);

        component.getProperties().remove(juce::Identifier("flexWidthCache"));
        component.getProperties().remove(juce::Identifier("flexHeightCache"));
        component.getProperties().remove(juce::Identifier("flexIndexCache"));

    }
    else
    {

        juce::FlexItem* fi = getItem(component);

        component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(fi->order));
        component.getProperties().set(juce::Identifier("flexWidthCache"), juce::var(component.getWidth()));
        component.getProperties().set(juce::Identifier("flexHeightCache"), juce::var(component.getHeight()));

        items.remove(fi);

    }

    applyLayout();

}
