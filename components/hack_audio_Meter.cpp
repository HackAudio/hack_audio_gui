inline float clip(float x)
{
    return std::max(0.0f, std::min(1.0f, x));
}

HackAudio::Meter::Meter()
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId, HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId, HackAudio::Colours::Cyan);

    pipsShown = true;

    pipSize = 4;

    pipLocations.resize(10);

    meterSkewFactor = 0.0;

    meterPeakStatus = false;
    currentPeakPos = 0.0;

    meterStyle = Vertical;

    meterRise = 0;
    meterOvershoot = 0.0f;
    meterFall = 0;

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

HackAudio::Meter::MeterStyle HackAudio::Meter::getMeterStyle() const
{

    return meterStyle;

}

void HackAudio::Meter::setMeterCalibration(HackAudio::Meter::MeterCalibration calibration)
{

    switch (calibration)
    {
        case Peak:
            setRiseTime(10);
            setFallTime(1500);
            setOvershoot(0);
            setPeakStatus(false);
            break;

        case RMS:
            setRiseTime(10);
            setPeakStatus(true);
            break;

        case VU:
            setRiseTime(300);
            setOvershoot(1.25);
            setFallTime(300);
            setPeakStatus(true);
            break;

        default:
            break;
    }

    meterCalibration = calibration;

}

HackAudio::Meter::MeterCalibration HackAudio::Meter::getMeterCalibration() const
{

    return meterCalibration;

}

void HackAudio::Meter::setSource(int channel, float* const source)
{

    meterSources.set(channel, source);
    meterBuffers.set(channel, 0.0f);

    if (!isTimerRunning())
    {
        startTimerHz(ANIMATION_FPS);
    }

    repaint();

}

void HackAudio::Meter::clearSource(int channel)
{

    meterSources.remove(channel);
    meterBuffers.remove(channel);

    repaint();

}

void HackAudio::Meter::clearSources()
{

    meterSources.clear();
    meterBuffers.clear();

    stopTimer();
    
}

void HackAudio::Meter::setSkewFactor(double factor, bool symmetricSkew)
{

    meterSkewFactor = factor;

}

void HackAudio::Meter::setSkewFactorFromMidPoint(double meterValueToShowAtMidPoint)
{

}

double HackAudio::Meter::getSkewFactor() const
{

    return meterSkewFactor;

}

void HackAudio::Meter::setPipState(bool shouldBeShown)
{

    pipsShown = shouldBeShown;
    resized();
    repaint();

}

bool HackAudio::Meter::getPipState() const
{

    return pipsShown;

}

void HackAudio::Meter::setPipCount(int count)
{

    pipLocations.resize(count);
    resized();
    repaint();

}

int HackAudio::Meter::getPipCount() const
{

    return pipLocations.size();

}

void HackAudio::Meter::setPeakStatus(bool shouldShowPeak)
{

    meterPeakStatus = shouldShowPeak;

}

bool HackAudio::Meter::getPeakStatus() const
{

    return meterPeakStatus;

}

void HackAudio::Meter::setRiseTime(int riseMs)
{

    assert(riseMs >= 0);

    meterRise = riseMs;

    meterCalibration = Custom;

}

int HackAudio::Meter::getRiseTime() const
{

    return meterRise;

}

void HackAudio::Meter::setOvershoot(float overshoot)
{

    assert(overshoot >= 0.0f);

    meterOvershoot = overshoot;

    meterCalibration = Custom;

}

float HackAudio::Meter::getOvershoot() const
{

    return meterOvershoot;

}

void HackAudio::Meter::setFallTime(int fallMs)
{

    assert(fallMs >= 0);

    meterFall = fallMs;

    meterCalibration = Custom;

}

int HackAudio::Meter::getFallTime() const
{

    return meterFall;

}

void HackAudio::Meter::setPipScale()
{

    int pipSizeCheck;

    if (meterStyle == Vertical)
    {

        pipSizeCheck = (indicatorArea.getHeight() - pipSize * 4) / (float)(pipLocations.size() - 1);

    }
    else
    {

        pipSizeCheck = (indicatorArea.getWidth() - pipSize * 4) / (float)(pipLocations.size() - 1);

    }

    pipSize = std::min(pipSizeCheck, 4);

}

void HackAudio::Meter::mouseUp(const juce::MouseEvent& e)
{

    currentPeakPos = 0.0;

}

void HackAudio::Meter::timerCallback()
{

    if (meterSources.size() == 0)
    {
        stopTimer();
    }

    float rise = (float)meterRise / 1000.0f;
    float fall = (float)meterFall / 1000.0f;

    for (int i = 0; i < meterSources.size(); ++i)
    {

        float in   = std::abs(*meterSources[i]);
        float last = meterBuffers[i];
        float peak = meterPeaks[i];
        float out;

        float ga = exp(-1.0f / (float)(ANIMATION_FPS * 0.01f));
        float gr = exp(-1.0f / (float)(ANIMATION_FPS * 1.5f));

        float g;

        if (peak < in)
        {
            g = ga;
        }
        else
        {
            g = gr;
        }

        peak = (1.0f - g) * in + g * peak;
        meterPeaks.set(i, clip(peak));

        if (meterCalibration == Peak || meterCalibration == Custom)
        {

            in   = std::abs(*meterSources[i]);
            last = meterBuffers[i];

            ga = exp(-1.0f / (float)(ANIMATION_FPS * rise));
            gr = exp(-1.0f / (float)(ANIMATION_FPS * fall));

            if (last < in)
            {
                g = ga;
            }
            else
            {
                g = gr;
            }

            out = (1.0f - g) * in + g * last;

        }
        else if (meterCalibration == VU)
        {

            in   = fmax(0.0f, *meterSources[i]);
            last = meterBuffers[i];
            g = exp(-1.0f / (float)(ANIMATION_FPS * rise));

            out = (1.0f - g) * in + g * last;

        }
        else if (meterCalibration == RMS)
        {

            in   = (*meterSources[i]) * (*meterSources[i]);
            last = meterBuffers[i];
            g = exp(-1.0f / (float)(ANIMATION_FPS * rise));

            out = (1.0f - g) * in + g * last;

        }

        meterBuffers.set(i, clip(out));
        repaint(indicatorArea);

    }

}

void HackAudio::Meter::paint(juce::Graphics& g)
{

    int width  = getWidth();
    int height = getHeight();

    juce::Path b;
    b.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(b);

    g.setColour(findColour(HackAudio::backgroundColourId));
    g.fillRect(indicatorArea);

    if (meterSources.size())
    {

        if (meterStyle == Vertical)
        {

            int channelWidth = indicatorArea.getWidth() / meterSources.size();

            for (int channel = 0; channel < meterSources.size(); ++channel)
            {

                float output = meterBuffers[channel];
                float peak   = meterPeaks[channel];

                g.setColour(findColour(HackAudio::highlightColourId));

                g.fillRect
                (
                   indicatorArea.getX() + channelWidth * channel,
                   indicatorArea.getBottom() - (int)(indicatorArea.getHeight() * output),
                   channelWidth,
                   (int)(indicatorArea.getHeight() * output)
                );

                if (meterPeakStatus)
                {

                    g.setColour(findColour(HackAudio::foregroundColourId));

                    g.drawLine
                    (
                        indicatorArea.getX() + channelWidth * channel,
                        indicatorArea.getBottom() - (indicatorArea.getHeight() * peak),
                        (indicatorArea.getX() + (channelWidth * channel)) + channelWidth,
                        indicatorArea.getBottom() - (indicatorArea.getHeight() * peak),
                        2
                    );
                    
                }

                g.setColour(findColour(HackAudio::backgroundColourId));
                g.drawLine
                (
                    (indicatorArea.getX() + channelWidth * channel) + channelWidth,
                    indicatorArea.getY(),
                    (indicatorArea.getX() + channelWidth * channel) + channelWidth,
                    indicatorArea.getBottom(),
                    1
                );

            }

        }
        else if (meterStyle == Horizontal)
        {

            int channelHeight = indicatorArea.getHeight() / meterSources.size();

            for (int channel = 0; channel < meterSources.size(); ++channel)
            {

                float output = meterBuffers[channel];
                float peak   = meterPeaks[channel];

                g.setColour(findColour(HackAudio::highlightColourId));

                g.fillRect
                (
                    indicatorArea.getX(),
                    indicatorArea.getY() + channelHeight * channel,
                    (int)(indicatorArea.getWidth() * output),
                    channelHeight
                );

                if (meterPeakStatus)
                {

                    g.setColour(findColour(HackAudio::foregroundColourId));

                    g.drawLine
                    (
                        indicatorArea.getX() + indicatorArea.getWidth() * peak,
                        indicatorArea.getY() + channelHeight * channel,
                        indicatorArea.getX() + indicatorArea.getWidth() * peak,
                        (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                        2
                    );

                }

                g.setColour(findColour(HackAudio::backgroundColourId));
                g.drawLine
                (
                    indicatorArea.getX(),
                    (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                    indicatorArea.getRight(),
                    (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                    1
                );

            }

        }

    }

    g.setColour(findColour(HackAudio::midgroundColourId));
    for (int i = 0; i < pipLocations.size(); ++i)
    {

        juce::Point<int>& pip = pipLocations.getReference(i);
        g.fillEllipse(pip.x - pipSize/2, pip.y - pipSize/2, pipSize, pipSize);

    }

    juce::Path p;
    p.addRoundedRectangle(4, 4, width - 8, height - 8, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.strokePath(p, juce::PathStrokeType(8));

}

void HackAudio::Meter::resized()
{

    int width  = getWidth();
    int height = getHeight();

    indicatorArea.setBounds(8, 8, width - 16, height - 16);

    for (int i = 0; i < pipLocations.size(); ++i)
    {

        juce::Point<int>& p = pipLocations.getReference(i);

        if (meterStyle == Vertical)
        {
            p.setXY(width / 2, (pipSize * 2) + ((indicatorArea.getHeight() - pipSize * 2) / (float)(pipLocations.size() - 1) * i));
        }
        else
        {
            p.setXY((pipSize * 2) + ((indicatorArea.getWidth() - pipSize * 2) / (float)(pipLocations.size() - 1) * i), height / 2);
        }

    }

    setPipScale();

}

