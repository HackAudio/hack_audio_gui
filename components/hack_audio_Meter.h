#ifndef HACK_AUDIO_METER_H
#define HACK_AUDIO_METER_H

namespace HackAudio
{

class Meter : public juce::Component,
              private juce::Timer
{
public:

    enum MeterStyle
    {
        Vertical,
        Horizontal
    };

    enum MeterCalibration
    {
        Peak,   // Peak Program Meter (PPM)
        RMS,
        VU,
        Custom,
    };

    Meter();
    ~Meter();

    /**
     Sets the current style to display the meter in
    */
    void setMeterStyle(MeterStyle style);

    /**
     Returns the current meter's style
    */
    MeterStyle getMeterStyle();

    /**
     Sets the calibration for the meter based off a MeterCalibration preset
    */
    void setMeterCalibration(MeterCalibration calibration);

    /**
     Returns the current meter calibration preset. If the meter has been calibrated by hand with the calibration methods, this will return MeterCalibration::Custom
    */
    MeterCalibration getMeterCalibration();

    /**
     Sets the float value to listen for
     
     @param source  a value ranging from 0.0 to 1.0
    */
    void setSource(int channel, float* const source);

    /**
     Stops the meter from listening to a source
    */
    void clearSource(int channel);

    /**
     Stops the meter from listening to all sources
    */
    void clearSources();

    /**
     Sets up a skew factor to alter the way values are distributed
    */
    void setSkewFactor(double factor, bool symmetricSkew=false);

    /**
     Sets up a skew factor to alter the way values are distributed
    */
    void setSkewFactorFromMidPoint(double meterValueToShowAtMidPoint);

    /**
     Returns the current skew factor
    */
    double getSkewFactor();

    /**
        Sets the visibility state of the meter's decorative pips
    */
    void setPipState(bool shouldBeShown);

    /**
        Returns the current visibility of the meter's pips
    */
    bool getPipState();

    /**
        Sets the number of pips the meter should display, adjusting their spacing automatically
    */
    void setPipCount(int count);

    /**
        Returns the current number of pips for the slider
    */
    int  getPipCount();

    /**
     Determines whether or not to show a line denoting the current peak value (will have no noticeable effect on a peak calibrated meter)
    */
    void setPeakStatus(bool shouldShowPeak);

    /**
     Returns the current status for displaying the peak value
    */
    bool getPeakStatus();

    /**
     Sets the time in milliseconds it takes for the meter to reach its target value. Set this to 0 for peak metering
    */
    void setRiseTime(int riseMs);

    /**
     Returns the current meter's rise time
    */
    int getRiseTime();

    /**
     Enables an overshoot value for VU style metering
     
     @param overshoot   a percentage of overshoot (e.g. 5.7)
    */
    void setOvershoot(float overshoot);

    /**
     Return the meter's current overshoot value;
    */
    float getOvershoot();

    /**
     Sets the time in milliseconds it takes for the meter to reach 0 from its current value. Set this to 0 for peak metering
    */
    void setFallTime(int fallMs);

    /**
     Returns the meter's current fall time
    */
    int getFallTime();

private:

    void setPipScale();

    void mouseUp(const juce::MouseEvent& e) override;

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    bool pipsShown;
    int pipSize;
    double meterSkewFactor;
    juce::Array<juce::Point<int>> pipLocations;

    juce::Rectangle<int> indicatorArea;

    bool meterPeakStatus;
    double currentPeakPos;

    int   meterRise;
    float meterOvershoot;
    int   meterFall;

    juce::Array<float*> meterSources;
    juce::Array<float>  meterBuffers;
    juce::Array<float>  meterPeaks;

    MeterStyle       meterStyle;
    MeterCalibration meterCalibration;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)

};

}

#endif
