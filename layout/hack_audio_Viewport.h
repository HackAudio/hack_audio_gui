#ifndef HACK_AUDIO_VIEWPORT_H
#define HACK_AUDIO_VIEWPORT_H

namespace HackAudio
{

/**
 An unbounded, infinitely-scrollable viewport for displaying HackAudio::Diagram components
*/

class Viewport : public juce::Component,
                 private juce::ComponentListener,
                 private juce::ButtonListener
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

    struct TraversalButton : public juce::Button
    {
        TraversalButton() : juce::Button("") {}
        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override {};
    };

    void traverseDown(HackAudio::Diagram* d);
    void traverseUp();
    void traverseTop();

    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;

    void buttonClicked(juce::Button* b) override;

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool resizeGuard;
    juce::Component contentContainer;
    HackAudio::Diagram* currentContent;

    juce::Array<HackAudio::Diagram*> parentContent;

    TraversalButton backButton;
    TraversalButton topButton;

    juce::ComponentDragger componentDragger;
    juce::ComponentAnimator componentAnimator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viewport)

};

}

#endif
