#ifndef HACK_AUDIO_SLIDER
#define HACK_AUDIO_SLIDER

namespace HackAudio
{

/**

 A custom slider using the generic juce::Slider API while adding custom styling and new methods.

*/

class Slider : public juce::Slider,
               public juce::Timer,
               public juce::SliderListener
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
    bool getPipState();

    /**
        Sets the number of pips the slider should display, adjusting their spacing automatically
    */
    void setPipCount(int count);

    /**
        Returns the current number of pips for the slider
    */
    int  getPipCount();

    /**
        Sets the size range slider pips will be drawn at
     
        @parameter minSize  the size of inactive pips (pip < sliderValue)
        @parameter maxSize  the size of active pips (pip > sliderValue)
    */
    void setPipSize(int minSize, int maxSize);

private:

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp  (const juce::MouseEvent& e) override;

    void timerCallback() override;

    void sliderValueChanged(juce::Slider*) override;

    void parentHierarchyChanged() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    bool isAnimating;
    float animationAcc;
    float animationVel;
    juce::Point<int> animationStart;
    juce::Point<int> animationEnd;

    bool resizeGuard;

    bool pipsShown;
    int pipSize;
    int minPipSize;
    int maxPipSize;

    juce::Array<juce::Point<int>> pipLocations;
    juce::Rectangle<int> trackArea;
    juce::Rectangle<int> thumbArea;
    juce::Rectangle<int> thumbSpan;
    juce::Rectangle<int> indicatorArea;

    bool isDraggable;
    bool isSettable;

    bool hasDefault;
    double sliderDefault;

};

}

#endif
