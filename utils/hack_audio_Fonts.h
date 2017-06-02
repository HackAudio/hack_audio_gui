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

/**

    The default font heights used in the HackAudio theme

*/
enum FontHeights
{

    Small  = 12,
    Medium = 14,
    Large  = 18,
    Huge   = 24,

};

}

#endif
