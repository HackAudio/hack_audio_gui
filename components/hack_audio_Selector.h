#ifndef HACK_AUDIO_SELECTOR_H
#define HACK_AUDIO_SELECTOR_H

#include "../utils/hack_audio_NavigationButton.h"

namespace HackAudio
{

class Selector : public juce::Component,
                 private juce::Timer
{
public:

    Selector();
    ~Selector();

    /**
     Sets the style of justification to be used for positioning the text
    */
    void setJustificationType(juce::Justification justification);

    /**
     Returns the current justification type
    */
    juce::Justification getJustificationType();

    /**
     Adds an item to the list of the selector choices
     
     @parameter newItemText     the string of the new item
     @parameter itemIndex       where to place the item, use -1 to place the new item last in the array
    */
    void addItem(const juce::String& newItemText, int itemIndex = -1);

    /**
     Adds an array of strings as new selector choices
     
     @parameter items       an array of strings
     @parameter firstItemOffset     where to begin placing the new choices, use -1 to place them last in the array
    */
    void addItemList(const juce::StringArray& items, int firstItemOffset = -1);

    /**
     Changes the text of one of the exisiting items
    */
    void changeItemText(int itemId, const juce::String& newText);

    /**
     Clears all choices from the selector
    */
    void clear(juce::NotificationType notification = juce::sendNotificationAsync);

    /**
     Manually sets the current choice of the selector
    */
    void setCurrentIndex(int index);

    /**
     Returns the index of the currently displayed item
    */
    int getCurrentIndex();

    /**
     Returns the total number of items the selector holds
    */
    int getNumItems();

    /**
     Returns the text of the currently displayed item
    */
    juce::String getCurrentText();

    /**
     Returns the text of the item at a specific index
    */
    juce::String getItemText(int index);

    /**
     Searches the choices and returns the index if it finds the given string
    */
    int getIndexForItem(const juce::String& itemText);

private:

    void mouseUp(const juce::MouseEvent& e) override;

    bool keyPressed(const juce::KeyPress& key) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Justification selectorJustification;

    int currentIndex;
    juce::Array<juce::String> selectorItems;

    juce::LinearSmoothedValue<float> colourInterpolation;

    HackAudio::NavigationButton selectLeft, selectRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Selector)

};

}

#endif
