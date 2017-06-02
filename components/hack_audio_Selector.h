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
    juce::Justification getJustificationType() const;

    /**
     Adds an item to the list of the selector choices
     
     @param newItemText     the string of the new item
     @param itemIndex       where to place the item, use -1 to place the new item last in the array
    */
    void addItem(const juce::String& newItemText, int itemIndex = -1);
    void addItem(const char* newItemText, int itemIndex = -1);

    /**
     Adds an array of strings as new selector choices
     
     @param items       an array of strings
     @param firstItemOffset     where to begin placing the new choices, use -1 to place them last in the array
    */
    void addItemList(const juce::StringArray& items, int firstItemOffset = -1);

    /**
     Changes the text of one of the exisiting items
    */
    void changeItemText(int itemId, const juce::String& newText);
    void changeItemText(int itemId, const char* newText);

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
    int getCurrentIndex() const;

    /**
     Returns the total number of items the selector holds
    */
    int getNumItems() const;

    /**
     Returns the text of the currently displayed item
    */
    juce::String getCurrentText() const;

    /**
     Returns the text of the item at a specific index
    */
    juce::String getItemText(int index) const;

    /**
     Searches the choices and returns the index if it finds the given string
    */
    int getIndexForItem(const juce::String& itemText) const;
    int getIndexForItem(const char* itemText) const;

    /**
     Sets the selector's font
    */
    void setFont(juce::Font font);

    /**
     Returns the selector's current font
    */
    juce::Font getFont() const;

    /**
        A class for receiving callbacks from a selector.
     
        To be told when a selector changes, you can register a Selector::Listener
        object using Selector::addListener()
     
        @see Selector::addListener, Selector::removeListener
    */
    class Listener
    {
    public:
        virtual ~Listener() {}

        /**
         Called when a selector moves forwards or backwards
        */
        virtual void selectorIndexChanged(Selector*, int index) = 0;

    };

    /**
    */
    void addListener(Listener* listener);

    /**
    */
    void removeListener(Listener* listener);

private:

    void indexChanged();

    void mouseUp(const juce::MouseEvent& e) override;

    bool keyPressed(const juce::KeyPress& key) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost  (juce::Component::FocusChangeType cause) override;

    void enablementChanged() override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::ListenerList<Listener> listeners;

    juce::Font selectorFont;

    juce::Justification selectorJustification;

    int currentIndex;
    juce::Array<juce::String> selectorItems;

    juce::LinearSmoothedValue<float> colourInterpolation;

    HackAudio::NavigationButton selectLeft, selectRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Selector)

};

}

#endif
