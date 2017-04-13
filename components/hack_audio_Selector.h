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

    void setJustificationType(juce::Justification justification);

    juce::Justification getJustificationType();

    void addItem(const juce::String& newItemText, int itemIndex = -1);

    void addItemList(const juce::StringArray& items, int firstItemOffset = -1);

    void changeItemText(int itemId, const juce::String& newText);

    void clear(juce::NotificationType notification = juce::sendNotificationAsync);

    void setCurrentIndex(int index);

    int getCurrentIndex();

    int getNumItems();

    juce::String getCurrentText();

    juce::String getItemText(int index);

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
