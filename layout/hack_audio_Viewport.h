#ifndef HACK_AUDIO_VIEWPORT_H
#define HACK_AUDIO_VIEWPORT_H

#include "../utils/hack_audio_NavigationButton.h"

namespace HackAudio
{

/**
 An unbounded, infinitely-scrollable viewport for displaying HackAudio::Diagram components
*/

class Viewport : public juce::Component,
                 private juce::ComponentListener,
                 private juce::ButtonListener,
                 private juce::Timer
{

    class Diagram;

public:

    Viewport();
    ~Viewport();

    /**
     Sets the top-level diagram to display
    */
    void setDiagram(HackAudio::Diagram& d);

private:

    void traverseDown(HackAudio::Diagram* d);
    void traverseUp();
    void traverseTop();

    void mouseEnter    (const juce::MouseEvent& e) override;
    void mouseExit     (const juce::MouseEvent& e) override;
    void mouseMove     (const juce::MouseEvent& e) override;
    void mouseDown     (const juce::MouseEvent& e) override;
    void mouseDrag     (const juce::MouseEvent& e) override;
    void mouseUp       (const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;

    void buttonClicked(juce::Button* b) override;

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool resizeGuard;
    juce::Component contentContainer;
    HackAudio::Diagram* currentContent;

    juce::Array<HackAudio::Diagram*> parentContent;

    NavigationButton backButton;
    NavigationButton topButton;

    juce::ComponentDragger componentDragger;
    juce::ComponentAnimator componentAnimator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viewport)

};

}

#endif
