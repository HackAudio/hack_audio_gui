#ifndef HACK_AUDIO_COLOURS_H
#define HACK_AUDIO_COLOURS_H

namespace HackAudio
{

/**

    Contains the predefined colours for the HackAudio theme
 
*/
class Colours
{
public:
    static const juce::Colour
    Black,
    DarkGray,
    Gray,
    LightGray,
    White,

    Teal,
    Cyan,
    Violet,
    Magenta;

private:
    Colours();

    JUCE_DECLARE_NON_COPYABLE (Colours)

};

/**
 The standard colour IDs for the HackAudio theme
*/
enum ColourIds
{

    backgroundColourId,
    midgroundColourId,
    foregroundColourId,
    highlightColourId
    
};
    
}

#endif
