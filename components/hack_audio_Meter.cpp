#include "hack_audio_Meter.h"

HackAudio::Meter::Meter()
{

    setColour(HackAudio::ColourIds::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::ColourIds::midgroundColourId, HackAudio::Colours::Gray);
    setColour(HackAudio::ColourIds::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::ColourIds::highlightColourId, HackAudio::Colours::Cyan);

    pipsShown = true;

    meterSkewFactor = 0.0;

    meterPeakStatus = false;
    currentPeakPos = 0.0;

    meterRise = 0;
    meterOvershoot = 0.0f;
    meterFall = 0;

    currentValue.reset(10, 0.01);

}

HackAudio::Meter::~Meter()
{

}

void HackAudio::Meter::setMeterStyle(HackAudio::Meter::MeterStyle style)
{

    meterStyle = style;
    resized();
    repaint();

}

HackAudio::Meter::MeterStyle HackAudio::Meter::getMeterStyle()
{

    return meterStyle;

}

void HackAudio::Meter::setMeterCalibration(HackAudio::Meter::MeterCalibration calibration)
{

    switch (calibration)
    {
        case Peak:
            setRiseTime(0);
            setPeakStatus(false);
            break;

        case RMS:
            setPeakStatus(true);
            break;

        case VU:
            setRiseTime(300);
            setOvershoot(1.25);
            setFallTime(300);
            setPeakStatus(false);
            break;

        default:
            break;
    }

    meterCalibration = calibration;

}

HackAudio::Meter::MeterCalibration HackAudio::Meter::getMeterCalibration()
{

    return meterCalibration;

}

void HackAudio::Meter::setSource(float* const source)
{

    valueSource = (double*)source;

    if (!isTimerRunning())
    {
        startTimerHz(ANIMATION_FPS);
    }

}

void HackAudio::Meter::setSource(double* const source)
{

    valueSource = source;

    if (!isTimerRunning())
    {
        startTimerHz(ANIMATION_FPS);
    }

}

void HackAudio::Meter::clearSource()
{

    valueSource = nullptr;
    currentValue.setValue(0.0);
    stopTimer();

}

void HackAudio::Meter::setSkewFactor(double factor, bool symmetricSkew)
{

    meterSkewFactor = factor;

}

void HackAudio::Meter::setSkewFactorFromMidPoint(double meterValueToShowAtMidPoint)
{

}

double HackAudio::Meter::getSkewFactor()
{

    return meterSkewFactor;

}

void HackAudio::Meter::setPipState(bool shouldBeShown)
{

    pipsShown = shouldBeShown;
    resized();
    repaint();

}

bool HackAudio::Meter::getPipState()
{

    return pipsShown;

}

void HackAudio::Meter::setPipCount(int count)
{

    pipLocations.resize(count);
    resized();
    repaint();

}

int HackAudio::Meter::getPipCount()
{

    return pipLocations.size();

}

void HackAudio::Meter::setPeakStatus(bool shouldShowPeak)
{

    meterPeakStatus = shouldShowPeak;

    meterCalibration = Custom;

}

bool HackAudio::Meter::getPeakStatus()
{

    return meterPeakStatus;

}

void HackAudio::Meter::setRiseTime(int riseMs)
{

    assert(riseMs > 0);

    meterRise = riseMs;

    meterCalibration = Custom;

}

int HackAudio::Meter::getRiseTime()
{

    return meterRise;

}

void HackAudio::Meter::setOvershoot(float overshoot)
{

    assert(overshoot > 0.0f);

    meterOvershoot = overshoot;

    meterCalibration = Custom;

}

float HackAudio::Meter::getOvershoot()
{

    return meterOvershoot;

}

void HackAudio::Meter::setFallTime(int fallMs)
{

    assert(fallMs > 0);

    meterFall = fallMs;

    meterCalibration = Custom;

}

int HackAudio::Meter::getFallTime()
{

    return meterFall;

}

void HackAudio::Meter::mouseUp(const juce::MouseEvent& e)
{

    currentPeakPos = 0.0;

}

void HackAudio::Meter::timerCallback()
{

    assert(valueSource);

    currentValue.setValue(*valueSource);

    repaint();

}

void HackAudio::Meter::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::ColourIds::backgroundColourId));
    g.fillPath(p);

    g.setColour(findColour(HackAudio::ColourIds::midgroundColourId));

    for (int i = 0; i < pipLocations.size(); ++i)
    {

        juce::Point<int>& pip = pipLocations.getReference(i);

        g.fillEllipse(pip.x - pipSize/2, pip.y - pipSize/2, pipSize, pipSize);

    }

    g.setColour(findColour(HackAudio::ColourIds::highlightColourId));
    g.fillRect(0, height - (height * currentValue.getNextValue()), width, height * currentValue.getNextValue());

    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::ColourIds::backgroundColourId));
    g.strokePath(p, juce::PathStrokeType(8));

}

void HackAudio::Meter::resized()
{

    int width  = getWidth();
    int height = getHeight();

    for (int i = 0; i < pipLocations.size(); ++i)
    {

        juce::Point<int>& p = pipLocations.getReference(i);

        p.setXY(width / 2, height - ((float)height / (float)(pipLocations.size() - 1) * i));

    }

}

