#ifndef HACK_AUDIO_FLEXBOX_H
#define HACK_AUDIO_FLEXBOX_H

namespace HackAudio
{

/**

 A custom implementation of juce::FlexBox that provides an easier API for setting up a basic layout

*/

class FlexBox : public juce::FlexBox,
                public juce::Component
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
     Sets the bounds the FlexBox should use and performs a layout adjustment
     
     @parameter bounds  the bounds the FlexBox should conform to
    */
    void applyBounds(juce::Rectangle<int> bounds);

private:

    void applyLayout();

};

}

#endif
