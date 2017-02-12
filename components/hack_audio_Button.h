#ifndef HACK_AUDIO_BUTTON_H
#define HACK_AUDIO_BUTTON_H

class Button : public juce::Button,
               public juce::Timer
{
public:

    Button();
    ~Button();

    enum ButtonStyle
    {
        BarSingleton,
        BarToggle,
        SlidingToggle
    };

    void setButtonStyle(ButtonStyle style);
    ButtonStyle getButtonStyle();

private:

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp  (const juce::MouseEvent& e) override;

    void timerCallback() override;

    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    void resized() override;

	float animationAcc;
	float animationVel;
	juce::Point<int> animationStart;
	juce::Point<int> animationEnd;

    juce::LinearSmoothedValue<float> currentColourInterpolation;

	bool isResizing;
    juce::Rectangle<int> thumbArea;
	juce::Rectangle<int> trackArea;
	juce::Rectangle<int> indicatorArea;

    bool isDraggable;

    ButtonStyle buttonStyle;

};

#endif
