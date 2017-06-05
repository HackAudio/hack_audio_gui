#ifndef HACK_AUDIO_TEXTFORMATTING_H
#define HACK_AUDIO_TEXTFORMATTING_H

#include <regex>

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
            baseline -= currentHeight / 2;
        }
        else if (baseline < 0)
        {
            baseline += currentHeight / 4;
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

            baseline += currentHeight / 4;

        }
        else if (baseline < 0)
        {

            baseline -= currentHeight / 2;

        }
        else if (baseline == 0)
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

    int rows = 0;
    juce::String s_row = jstring;
    while (1)
    {

        rows++;
        s_row = s_row.fromFirstOccurrenceOf("&//", false, true);
        if (!s_row.contains("&//")) { break; }

    }

    juce::StringArray chars;

    while (1)
    {

        chars.add(jstring.upToFirstOccurrenceOf("&", false, true));
        if (!jstring.contains("&")) { break; }
        jstring = jstring.fromFirstOccurrenceOf("&", false, false);
        jstring = jstring.trim();

    }

    int spacing = 0;
    int n = 0;

    for (int j = 0; j < chars.size(); ++j)
    {

        juce::StringArray results = parseText(chars[j]);

        for (int k = 0; k < results.size(); ++k)
        {

            juce::String temp = results[k];

            if (temp.startsWith("//"))
            {

                baseline += height / rows;
                temp = temp.removeCharacters("//");
                temp = temp.trimStart();
                spacing = 0;
                offset = 0;
                n = 0;

            }
            else if (temp.startsWith("^") || temp.startsWith("_"))
            {

                juce::Rectangle<float> glyphOffset = glyphs.getBoundingBox(glyphs.getNumGlyphs() - 1, glyphs.getNumGlyphs(), true);
                offset = spacing + glyphOffset.getWidth();

                int tempBaseline = baseline;

                formatScript(glyphs, temp, font, currentHeight, tempBaseline, offset);

                currentHeight = font.getHeight();
                offset = 0;

                n--;

                continue;

            }

            spacing = ((width / rows) * n) - font.getStringWidth(temp);

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
