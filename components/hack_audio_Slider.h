#ifndef HACK_AUDIO_SLIDER
#define HACK_AUDIO_SLIDER

namespace HackAudio
{

/**

 A custom slider using the generic juce::Slider API while adding custom styling and new methods.

*/

class Slider : public juce::Slider,
               private juce::Timer,
               private juce::SliderListener
{
public:

    Slider();
    ~Slider();

    /**
        Sets whether the slider should snap to a value when double clicked and, if so, what value
     
        @param shouldHaveDefault    whether the slider accepts double clicks
     
        @param defaultValue         the value the slider should snap too (must be in slider's range)
    */
    void setDefaultValue(bool shouldHaveDefault, double defaultValue);

    /**
        Sets the visibility state of the slider's decorative pips
    */
    void setPipState(bool shouldBeShown);

    /**
        Returns the current visibility of the slider's pips
    */
    bool getPipState() const;

    /**
        Sets the number of pips the slider should display, adjusting their spacing automatically
    */
    void setPipCount(int count);

    /**
        Returns the current number of pips for the slider
    */
    int  getPipCount() const;

    /**
     Sets the size of the slider with a 1:1 aspect ratio. Useful for rotary sliders

     @paremeter size     the size that both the width and height will be set to
     */
    void setSymmetricSize(int size);

private:

    void setPipScale();

    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp  (const juce::MouseEvent& e) override;

    bool keyPressed(const juce::KeyPress& key) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost  (juce::Component::FocusChangeType cause) override;

    void enablementChanged() override;

    void timerCallback() override;

    void sliderValueChanged(juce::Slider*) override;

    void parentHierarchyChanged() override;

    void stopAnimation();

    void paint (juce::Graphics& g) override;
    void resized() override;

    bool isAnimating;
    float animationAcc;
    float animationVel;
    juce::Point<int> animationStart;
    juce::Point<int> animationEnd;

    bool resizeGuard;

    bool pipsShown;
    int minPipSize;
    int maxPipSize;
    int currentMinPipSize;
    int currentMaxPipSize;
    int pipClicked;

    juce::Array<juce::Point<int>> pipLocations;
    juce::Rectangle<int> trackArea;
    juce::Rectangle<int> thumbArea;
    juce::Rectangle<int> thumbSpan;
    juce::Rectangle<int> indicatorArea;

    bool isDraggable;
    bool isSettable;

    bool hasDefault;
    double sliderDefault;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider)

};

}

#endif
