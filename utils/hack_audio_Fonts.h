#ifndef HACK_AUDIO_FONTS
#define HACK_AUDIO_FONTS

namespace HackAudio
{

/**

    The default fonts for the HackAudio theme
 
*/
class Fonts
{
public:

    static const juce::Font
    NowLight,
    NowRegular,
    NowBold,
    Baumans;

private:

    Fonts();
    JUCE_DECLARE_NON_COPYABLE (Fonts)

};

}

#endif
