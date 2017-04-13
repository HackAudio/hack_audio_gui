#ifndef HACK_AUDIO_BUTTON_H
#define HACK_AUDIO_BUTTON_H

namespace HackAudio
{

/**

 A custom button using the generic juce::Slider API while adding custom styling and new methods.
 
 */

class Button : public juce::Button,
               private juce::Timer
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

    /**
     Sets the button's font style
    */
    void setFont(juce::Font font);

    /**
     Return the button's current font
    */
    juce::Font getFont();

private:

    void mouseMove(const juce::MouseEvent& e)  override {};
    void mouseEnter(const juce::MouseEvent& e) override {};
    void mouseExit(const juce::MouseEvent& e)  override {};

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

    juce::LinearSmoothedValue<float> colourInterpolation;

	bool resizeGuard;
    juce::Rectangle<int> thumbArea;
	juce::Rectangle<int> trackArea;
	juce::Rectangle<int> indicatorArea;

    bool isDraggable;

    juce::Font buttonFont;

    ButtonStyle buttonStyle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)

};

}

#endif
