#ifndef HACK_AUDIO_VIEWPORT_H
#define HACK_AUDIO_VIEWPORT_H

namespace HackAudio
{

/**
 An unbounded, infinitely-scrollable viewport for displaying HackAudio::Diagram components
*/

class Viewport : public juce::Component
{

    class Diagram;

public:

    Viewport();
    ~Viewport();

    /**
     Sets the currently displayed component, there may only be one at a time
    */
    void setViewedComponent(juce::Component* c);

private:

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool resizeGuard;
    juce::Component contentContainer;

    juce::ComponentDragger componentDragger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viewport)

};

}

#endif
