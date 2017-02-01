#ifndef HACK_AUDIO_SLIDER
#define HACK_AUDIO_SLIDER

class Slider : public juce::Slider,
               public juce::Timer
{
public:

    Slider();
    ~Slider();

    void setDefaultValue(double defaultValue);

private:

    void mouseDown(const juce::MouseEvent& e) override;

    void timerCallback() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    bool isAnimating;
    float animationAcc;
    float animationVel;
    juce::Point<int> animationStart;
    juce::Point<int> animationEnd;

    bool isResizing;
    juce::Array<juce::Rectangle<int>> pipAreas;
    juce::Rectangle<int> sliderArea;
    juce::Rectangle<int> thumbArea;

};

#endif
