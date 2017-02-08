#ifndef HACK_AUDIO_BUTTON_H
#define HACK_AUDIO_BUTTON_H

class Button : public juce::Button,
               public juce::Timer
{
public:

    Button();
    ~Button();

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

	bool isResizing;
    juce::Rectangle<int> thumbArea;
	juce::Rectangle<int> trackArea;
	juce::Rectangle<int> indicatorArea;

    bool isDraggable;

};

#endif
