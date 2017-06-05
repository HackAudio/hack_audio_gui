/* Copyright (C) 2017 by Antonio Lassandro, HackAudio LLC
 *
 * hack_audio_gui is provided under the terms of The MIT License (MIT):
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef HACK_AUDIO_METER_H
#define HACK_AUDIO_METER_H

namespace HackAudio
{

/**
 A custom meter component used to measure audio signals 
*/
class Meter : public juce::Component,
              private juce::Timer
{
public:

    /**
     The available HackAudio::Meter styles
     
     @see setMeterStyle
    */
    enum MeterStyle
    {
        Vertical,       /**< A vertically facing meter */
        Horizontal      /**< A horizontally facing meter */
    };

    /**
     Identifiers for easily setting meter calibration presets
    */
    enum MeterCalibration
    {
        Peak,           /**< Peak Program Meter (PPM), responds very quickly to the absolute digital levels of the audio signal */
        RMS,            /**< Root-Mean-Square (RMS), a display of the overall average level of the incoming signal */
        VU,             /**< Volume-Units (VU), based on specifications of analog VU meters where 0VU = +4 dBu */
        Custom,         /**< Custom is automatically assigned whenever meter attributes are manually set */
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
    MeterStyle getMeterStyle() const;

    /**
     Sets the calibration for the meter based off a MeterCalibration preset
    */
    void setMeterCalibration(MeterCalibration calibration);

    /**
     Returns the current meter calibration preset. If the meter has been calibrated by hand with the calibration methods, this will return MeterCalibration::Custom
    */
    MeterCalibration getMeterCalibration() const;

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
    double getSkewFactor() const;

    /**
        Sets the visibility state of the meter's decorative pips
    */
    void setPipState(bool shouldBeShown);

    /**
        Returns the current visibility of the meter's pips
    */
    bool getPipState() const;

    /**
        Sets the number of pips the meter should display, adjusting their spacing automatically
    */
    void setPipCount(int count);

    /**
        Returns the current number of pips for the slider
    */
    int  getPipCount() const;

    /**
     Determines whether or not to show a line denoting the current peak value (will have no noticeable effect on a peak calibrated meter)
    */
    void setPeakStatus(bool shouldShowPeak);

    /**
     Returns the current status for displaying the peak value
    */
    bool getPeakStatus() const;

    /**
     Sets the time in milliseconds it takes for the meter to reach its target value. Set this to 0 for peak metering
    */
    void setRiseTime(int riseMs);

    /**
     Returns the current meter's rise time
    */
    int getRiseTime() const;

    /**
     Enables an overshoot value for VU style metering
     
     @param overshoot   a percentage of overshoot (e.g. 5.7)
    */
    void setOvershoot(float overshoot);

    /**
     Return the meter's current overshoot value;
    */
    float getOvershoot() const;

    /**
     Sets the time in milliseconds it takes for the meter to reach 0 from its current value. Set this to 0 for peak metering
    */
    void setFallTime(int fallMs);

    /**
     Returns the meter's current fall time
    */
    int getFallTime() const;

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
