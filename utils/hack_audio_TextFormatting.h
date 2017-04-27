#ifndef HACK_AUDIO_TEXTFORMATTING_H
#define HACK_AUDIO_TEXTFORMATTING_H

#include <regex>

juce::StringArray parseText(juce::String stringToParse)
{

    juce::StringArray results;

    std::regex r("\\\\array.+");

    std::string s(stringToParse.toUTF8());

    std::sregex_iterator begin(s.begin(), s.end(), r), end;
    for (auto i = begin; i != end; ++i)
    {
        results.add(i->str());
    }

    if (!results.size())
    {
        r.assign("([^\\^_]+|([\\^_][^\\s\\^_]){2}|[\\^_]\\{[^\\}]+\\}|[\\^_][^\\s\\^_]?)");

        std::sregex_iterator begin(s.begin(), s.end(), r), end;
        for (auto i = begin; i != end; ++i)
        {
            results.add(i->str());
        }
    }
    
    return results;
    
}

void formatScript(juce::GlyphArrangement& glyphs, juce::String jstring, juce::Font& font, int& currentHeight, int& baseline, int& offset)
{

    if (jstring.startsWith("^"))
    {

        if (baseline > 0)
        {
            baseline -= baseline / 2;
        }
        else if (baseline < 0)
        {
            baseline += baseline / 4;
        }
        else
        {
            baseline -= currentHeight / 2;
        }

        currentHeight -= currentHeight / 3;

        if (jstring.startsWith("^{"))
        {

            jstring = jstring.fromFirstOccurrenceOf("{", false, true);
            jstring = jstring.upToFirstOccurrenceOf("}", false, true);

            juce::StringArray temp = parseText(jstring);

            for (int i = 0; i < temp.size(); ++i)
            {

                if (temp[i].containsAnyOf("^_"))
                {

                    formatScript(glyphs, temp[i], font, currentHeight, baseline, offset);

                }
                else
                {

                    glyphs.addLineOfText
                    (
                     font.withHeight(currentHeight),
                     temp[i],
                     offset,
                     baseline
                    );

                    offset = (int)glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), true).getWidth();

                }

            }

        }
        else if (jstring.matchesWildcard("^*_*", true))
        {

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(1,2),
             offset,
             baseline
             );

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(3, 4),
             offset,
             baseline + baseline * -2
             );

        }
        else
        {

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(1),
             offset,
             baseline
             );

        }

    }
    else if (jstring.startsWith("_"))
    {

        if (baseline > 0)
        {
            baseline += baseline / 4;
        }
        else if (baseline < 0)
        {
            baseline -= baseline / 2;
        }
        else
        {
            baseline += currentHeight / 2;
        }

        currentHeight -= currentHeight / 3;

        if (jstring.startsWith("_{"))
        {

            jstring = jstring.fromFirstOccurrenceOf("{", false, true);
            jstring = jstring.upToFirstOccurrenceOf("}", false, true);

            juce::StringArray temp = parseText(jstring);

            for (int i = 0; i < temp.size(); ++i)
            {
                if (temp[i].containsAnyOf("^_"))
                {

                    formatScript(glyphs, temp[i], font, currentHeight, baseline, offset);
                    
                }
                else
                {

                    glyphs.addLineOfText
                    (
                     font.withHeight(currentHeight),
                     jstring,
                     offset,
                     baseline
                    );

                    offset = (int)glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), true).getWidth();

                }

            }

        }
        else if (jstring.matchesWildcard("_*^*", true))
        {

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(1,2),
             offset,
             baseline
             );

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(3, 4),
             offset,
             baseline + baseline * -2
             );

        }
        else
        {

            glyphs.addLineOfText
            (
             font.withHeight(currentHeight),
             jstring.substring(1),
             offset,
             baseline
             );

        }

    }

    offset = (int)glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), true).getWidth();

}

void formatArray(juce::GlyphArrangement& glyphs, juce::String& jstring, juce::Font& font, int& currentHeight, int& baseline, int& offset, int& width, int& height)
{

    offset = 0;
    baseline = 0;
    currentHeight = font.getHeight();

    jstring = jstring.fromFirstOccurrenceOf("\\array", false, true).upToFirstOccurrenceOf("\\end", false, true);
    jstring = jstring.trimStart().trimEnd();

    juce::StringArray columns;

    bool finished = false;
    while (!finished)
    {

        columns.add(jstring.upToFirstOccurrenceOf("&", false, true));
        finished = (jstring.contains("&")) ? false : true;
        jstring = jstring.fromFirstOccurrenceOf("&", false, false);
        jstring = jstring.trim();

    }

    int spacing = 0;
    int n = 0;

    for (int j = 0; j < columns.size(); ++j)
    {

        juce::StringArray results = parseText(columns[j]);

        for (int k = 0; k < results.size(); ++k)
        {

            juce::String temp = results[k];

            if (temp.startsWith("//"))
            {

                baseline += height / columns.size();
                temp = temp.removeCharacters("//");
                spacing = 0;
                offset = 0;
                n = 0;

            }
            else if (temp.startsWith("^") || temp.startsWith("_"))
            {

                offset = spacing + font.getStringWidth(" ") * 1.25;

                int tempBaseline = baseline;

                formatScript(glyphs, temp, font, currentHeight, tempBaseline, offset);

                currentHeight = font.getHeight();
                offset = 0;

                n--;

                continue;

            }

            spacing = ((width / columns.size()) * n) - font.getStringWidth(temp) / 2;

            glyphs.addLineOfText(font.withHeight(currentHeight), temp, spacing, baseline);

            n++;

        }

    }

}

juce::GlyphArrangement formatText(juce::String stringToFormat, juce::Font font, juce::Justification justificationType, int x, int y, int width, int height)
{

    juce::GlyphArrangement glyphs;

    int fontHeight    = font.getHeight();
    int currentHeight = fontHeight;

    int offset   = 0;
    int baseline = 0;

    juce::StringArray results = parseText(stringToFormat);

    for (int i = 0; i < results.size(); ++i)
    {

        juce::String jstring = results[i];

        if (jstring.startsWith("^") || jstring.startsWith("_"))
        {

            formatScript(glyphs, jstring, font, currentHeight, baseline, offset);

        }
        else if (jstring.startsWith("\\array"))
        {

            formatArray(glyphs, jstring, font, currentHeight, baseline, offset, width, height);

        }
        else
        {

            baseline = 0;
            currentHeight = fontHeight;

            glyphs.addLineOfText(font.withHeight(currentHeight), jstring, offset, baseline);
            
        }
        
        offset = (int)glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), true).getWidth();
        
    }
    
    glyphs.justifyGlyphs(0, glyphs.getNumGlyphs(), x, y, width, height, justificationType);

    return glyphs;
    
}


#endif
