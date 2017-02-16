#ifndef HACK_AUDIO_SLIDER
#define HACK_AUDIO_SLIDER

class Slider : public juce::Slider,
               public juce::Timer,
               public juce::SliderListener
{
public:

    Slider();
    ~Slider();

    void setDefaultValue(bool shouldHaveDefault, double defaultValue);

private:

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp  (const juce::MouseEvent& e) override;

    void timerCallback() override;

    void sliderValueChanged(juce::Slider*) override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    bool isAnimating;
    float animationAcc;
    float animationVel;
    juce::Point<int> animationStart;
    juce::Point<int> animationEnd;

    bool isResizing;
    juce::Array<juce::Rectangle<int>> pipAreas;
    juce::Rectangle<int> trackArea;
    juce::Rectangle<int> thumbArea;
    juce::Rectangle<int> thumbSpan;
    juce::Rectangle<int> indicatorArea;

    bool isDraggable;

    bool hasDefault;
    double sliderDefault;

};

#endif
