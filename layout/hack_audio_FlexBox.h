#ifndef HACK_AUDIO_FLEXBOX_H
#define HACK_AUDIO_FLEXBOX_H

namespace HackAudio
{

/**

 A custom implementation of juce::FlexBox that provides an easier API for setting up a basic layout

*/

class FlexBox : public juce::FlexBox,
                public juce::ComponentListener
{
public:

    FlexBox();
    ~FlexBox();

    /**
     Add a component to be controlled by the FlexBox (forces a layout adjusment)
     
     @paremeter component   the component to add
    */
    void addComponent(juce::Component* component);

    /**
     Remove a component from being controlled by the FlexBox
     
     @parameter component   a component currently controlled by the FlexBox
    */
    void removeComponent(juce::Component* component);

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
     Sets the bounds the FlexBox should use and performs a layout adjustment
     
     @parameter bounds  the bounds the FlexBox should conform to
    */
    void applyBounds(juce::Rectangle<int> bounds);

    /**
     Custom API for calling juce::FlexBox::performLayout(...) that handles component resizing callbacks
    */
    void applyLayout();

private:

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    bool resizeGuard;

    juce::Rectangle<int> flexBoxBounds;

};

}

#endif
