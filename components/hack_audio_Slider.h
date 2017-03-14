#ifndef HACK_AUDIO_SLIDER
#define HACK_AUDIO_SLIDER

namespace HackAudio
{

class Slider : public juce::Slider,
               public juce::Timer,
               public juce::SliderListener
{
public:

    Slider();
    ~Slider();

    void setDefaultValue(bool shouldHaveDefault, double defaultValue);

    void setPipState(bool shouldBeShown);
    bool getPipState();

    void setPipCount(int count);
    int  getPipCount();

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
