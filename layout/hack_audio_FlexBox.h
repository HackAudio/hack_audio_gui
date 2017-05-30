#ifndef HACK_AUDIO_FLEXBOX_H
#define HACK_AUDIO_FLEXBOX_H

namespace HackAudio
{

/**

 A custom implementation of juce::FlexBox that provides an easier API for setting up a basic layout

*/

class FlexBox : private juce::FlexBox,
                private juce::ComponentListener
{
public:

    FlexBox();
    ~FlexBox();

    /**
     Sets the default juce::FlexItem settings to use when adding new items
     
     @param item    the juce::FlexItem to use as default
     @param updateExistingItems whether or not to conform the current items to the passed default
    */
    void setDefaultItem(juce::FlexItem item, bool updateExistingItems);

    /**
     Add a component to be controlled by the FlexBox (forces a layout adjusment)
     
     @paremeter component   the component to add
    */
    void addComponent(juce::Component& component, int customOrder = 0);

    /**
     Remove a component from being controlled by the FlexBox
     
     @param component   a component currently controlled by the FlexBox
    */
    void removeComponent(juce::Component& component);

    /**
     Add a FlexBox to nest within this one
    */
    void addFlexBox(juce::FlexBox& flexbox, int customOrder = 0);

    /**
     Remove a flexbox from being controlled by this one
    */
    void removeFlexBox(juce::FlexBox& flexbox);

    /**
     Specifies how flex items are placed in the flex container, and defines the direction of the main axis
    */
    void setDirection(juce::FlexBox::Direction newDirection);

    /**
     Returns the FlexBox's current direction
     */
    juce::FlexBox::Direction getDirection() const;

    /**
     Specifies whether items are forced into a single line or can be wrapped onto multiple lines
    */
    void setWrap(juce::FlexBox::Wrap newWrap);

    /**
     Returns the FlexBox's current wrapping
     */
    juce::FlexBox::Wrap getWrap() const;

    /**
     Specifies how a flex container's lines are placed within the flex container when there is extra space on the cross-axis
    */
    void setAlignContent(juce::FlexBox::AlignContent newAlignment);

    /**
     Returns the FlexBox's current content alignment
     */
    juce::FlexBox::AlignContent getAlignContent() const;

    /**
     Specifies the alignment of flex items along the cross-axis of each line
    */
    void setAlignItems(juce::FlexBox::AlignItems newAlignment);

    /**
     Returns the FlexBox's current item alignment
    */
    juce::FlexBox::AlignItems getAlignItems() const;

    /**
     Defines how the container distributes space between and around items along the main-axis
    */
    void setJustifyContent(juce::FlexBox::JustifyContent newJustification);

    /**
     Returns the FlexBox's current justification
    */
    juce::FlexBox::JustifyContent getJustifyContent() const;

    /**
     Returns the FlexItems currently held by the FlexBox
    */
    juce::Array<juce::FlexItem> getItems() const;

    /**
     Returns the FlexItem associated with the given component
     
     @param component   a component controlled by the FlexBox
    */
    juce::FlexItem* getItem(juce::Component& component) const;

    /**
     Returns the FlexItem associated with the given FlexBox

     @param flexbox   a component controlled by this FlexBox
     */
    juce::FlexItem* getItem(juce::FlexBox& flexbox) const;

    /**
     Sets the properties for the FlexItem of the relevant component
     
     @param component   a component controlled by the FlexBox
     @param newFlexProperties   the properties the FlexItem should inherit
    */
    void setItem(juce::Component& component, juce::FlexItem newFlexProperties);

    /**
     Sets the properties for the FlexItem of the relevant FlexBox

     @param flexbox   a FlexBox controlled by this FlexBox
     @param newFlexProperties   the properties the FlexItem should inherit
     */
    void setItem(juce::FlexBox& flexbox, juce::FlexItem newFlexProperties);

    /**
     Set the flex for a component's FlexItem
    */
    void setItemFlex(juce::Component& component, float newFlexGrow);
    void setItemFlex(juce::Component& component, float newFlexGrow, float newFlexShrink);
    void setItemFlex(juce::Component& component, float newFlexGrow, float newFlexShrink, float newFlexBasis);

    /**
     Set the flex for a FlexBox's FlexItem
    */
    void setItemFlex(juce::FlexBox& flexbox, float newFlexGrow);
    void setItemFlex(juce::FlexBox& flexbox, float newFlexGrow, float newFlexShrink);
    void setItemFlex(juce::FlexBox& flexbox, float newFlexGrow, float newFlexShrink, float newFlexBasis);

    /**
     Set the width for a component's FlexItem
    */
    void setItemWidth(juce::Component& component, float newWidth);

    /**
     Set the width for a FlexBox's FlexItem
    */
    void setItemWidth(juce::FlexBox& flexbox, float newWidth);

    /**
     Set the flex for a component's FlexItem
    */
    void setItemHeight(juce::Component& component, float newHeight);

    /**
     Set the flex for a FlexBox's FlexItem
    */
    void setItemHeight(juce::FlexBox& flexbox, float newHeight);

    /**
     Set the margin for a component's FlexItem
    */
    void setItemMargin(juce::Component& component, juce::FlexItem::Margin newMargin);

    /**
     Set the margin for a FlexBox's FlexItem
    */
    void setItemMargin(juce::FlexBox& flexbox, juce::FlexItem::Margin newMargin);

    /**
     Sets the flex for all contained items
    */
    void applyItemFlex(float newFlexGrow);
    void applyItemFlex(float newFlexGrow, float newFlexShrink);
    void applyItemFlex(float newFlexGrow, float newFlexShrink, float newFlexBasis);

    /**
     Sets the width for all contained items
    */
    void applyItemWidth(float newWidth);

    /**
     Sets the height for all contained items
    */
    void applyItemHeight(float newHeight);

    /**
     Sets the margin for all contained items
    */
    void applyItemMargin(juce::FlexItem::Margin newMargin);

    /**
     Sets all FlexItem properties to those of newFlexProperties
    */
    void applyGlobalFlexItem(juce::FlexItem newFlexProperties);

    /**
     Sets the bounds the FlexBox should use and performs a layout adjustment
     
     @param bounds  the bounds the FlexBox should conform to
    */
    void applyBounds(juce::Rectangle<int> bounds);
    void applyBounds(juce::Rectangle<float> bounds);
    void applyBounds(int x, int y, int width, int height);

    /**
     Custom API for calling juce::FlexBox::performLayout(...) that handles component resizing callbacks
    */
    void applyLayout();

private:

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void componentVisibilityChanged(juce::Component& component) override;

    bool resizeGuard;

    bool validBounds;
    juce::Rectangle<int> flexBoxBounds;

    juce::FlexItem defaultFlexSettings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexBox)

};

}

#endif
