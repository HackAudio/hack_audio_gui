#ifndef HACK_AUDIO_BUTTON_H
#define HACK_AUDIO_BUTTON_H

namespace HackAudio
{

/**

 A custom button using the generic juce::Slider API while adding custom styling and new methods.
 
 */

class Button : public juce::Button,
               public juce::Timer
{
public:

    Button();
    ~Button();

    /**
     The available HackAudio::Button styles
     
     BarSingleton is a square button that saves no state and simply sends a singleton event
     
     BarToggle saves its state and colors itself accordingly
     
     SlidingToggle saves state and aesthetically emulates a toggle switch
    */
    enum ButtonStyle
    {
        BarSingleton,
        BarToggle,
        SlidingToggle
    };

    /**
     Sets the button's current style
     
     @see ButtonStyle
    */
    void setButtonStyle(ButtonStyle style);

    /**
     Return the button's current style
     
     @see ButtonStyle
    */
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

	bool resizeGuard;
    juce::Rectangle<int> thumbArea;
	juce::Rectangle<int> trackArea;
	juce::Rectangle<int> indicatorArea;

    bool isDraggable;

    int fontSize;

    ButtonStyle buttonStyle;

};

}

#endif
