#include "hack_audio_Button.h"

HackAudio::Button::Button() : juce::Button("")
{
	setButtonText("");
	setClickingTogglesState(true);
	setTriggeredOnMouseDown(false);
	isResizing = false;
}

HackAudio::Button::~Button()
{

}

void HackAudio::Button::mouseDown(const juce::MouseEvent& e)
{

	if (!trackArea.contains(e.getPosition()) && !thumbArea.contains(e.getPosition())) { return; }

	animationStart.setXY(thumbArea.getX(), thumbArea.getY());

	if (e.getMouseDownX() <= getWidth() / 2)
	{
        setToggleState(false, juce::sendNotification);
		animationEnd.setXY(32, 16);
	}
	else
	{
        setToggleState(true, juce::sendNotification);
		animationEnd.setXY(64, 16);
	}

	startTimerHz(60);

}

void HackAudio::Button::timerCallback()
{

	if (thumbArea.getX() <= (animationStart.getX() + animationEnd.getX()) / 2)
	{
		animationAcc += ANIMATION_SPEED;
	}
	else
	{
		animationAcc -= ANIMATION_SPEED;
	}

	if (std::abs(animationVel) < 16)
	{
		animationVel += animationAcc;
	}

	thumbArea.translate(animationVel, 0);
    indicatorArea.setWidth((thumbArea.getX() - indicatorArea.getX()) + thumbArea.getWidth()/2);

	if (thumbArea.getPosition().getDistanceFrom(animationEnd) < 8)
	{
		animationAcc = 0;
		animationVel = 0;

		thumbArea.setPosition(animationEnd);

		stopTimer();
        repaint();
	}

	repaint();

}

void HackAudio::Button::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    int width  = getWidth();
    int height = getHeight();

	//Draw Background
	juce::Path p;
	p.addRoundedRectangle(0, 0, width, height, 32, 32, false, true, true, false);
	g.setColour(HackAudio::Colours::Black);
	g.fillPath(p);

	p.clear();

    p.addRoundedRectangle(trackArea.getX(), trackArea.getY(), trackArea.getWidth(), trackArea.getHeight(), 8, 8, false, true, true, false);
    g.setColour(HackAudio::Colours::Gray);
    g.fillPath(p);

    p.clear();

	p.addRoundedRectangle(indicatorArea.getX(), indicatorArea.getY(), indicatorArea.getWidth(), indicatorArea.getHeight(), 8, 8, false, true, true, false);
	g.setColour(HackAudio::Colours::Cyan);
	g.fillPath(p);

	p.clear();

	p.addRoundedRectangle(thumbArea.getX(), thumbArea.getY(), thumbArea.getWidth(), thumbArea.getHeight(), 8, 8, false, true, true, false);
	g.setColour(HackAudio::Colours::White);
	g.fillPath(p);

	g.setColour(HackAudio::Colours::Black);
    g.strokePath(p, juce::PathStrokeType::PathStrokeType(8));

}

void HackAudio::Button::resized()
{
	if (isResizing) { return; }

	isResizing = true;

    trackArea.setBounds(32, 16, 64, 32);

	thumbArea.setSize(32, 32);
	indicatorArea.setPosition(32, 16);
	indicatorArea.setHeight(32);

	if (getToggleState())
	{
		thumbArea.setPosition(32, 16);
	}
	else
	{
		thumbArea.setPosition(64, 16);
	}

	indicatorArea.setWidth((thumbArea.getX() - indicatorArea.getX()) + thumbArea.getWidth()/2);

	setSize(128, 64);

	isResizing = false;
}
