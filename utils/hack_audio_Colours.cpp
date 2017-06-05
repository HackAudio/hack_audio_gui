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

const juce::Colour HackAudio::Colours::Black     (0xFF100B28);
const juce::Colour HackAudio::Colours::DarkGray  (0xFF26324A);
const juce::Colour HackAudio::Colours::Gray      (0xFF425568);
const juce::Colour HackAudio::Colours::LightGray (0xFF5F7584);
const juce::Colour HackAudio::Colours::White     (0xFFFFFFFF);

const juce::Colour HackAudio::Colours::Teal      (0xFF13F59B);
const juce::Colour HackAudio::Colours::Cyan      (0xFF00C8FF);
const juce::Colour HackAudio::Colours::Violet    (0xFFCE8AFF);
const juce::Colour HackAudio::Colours::Magenta   (0xFFFB7BCA);

void HackAudio::Colours::setGlobalColour(int colourId, juce::Colour newColour)
{

    std::function<void (juce::Component*)> traverse = [&](juce::Component* parent)
    {

        for (int i = 0; i < parent->getNumChildComponents(); ++i)
        {

            if (juce::Component* const child = parent->getChildComponent(i))
            {

                child->setColour(colourId, newColour);
                child->repaint();
                traverse(child);
                
            }
            
        }

    };

    juce::Desktop& d = juce::Desktop::getInstance();

    for (int i = 0; i < d.getNumComponents(); ++i)
    {

        if (juce::Component* c = d.getComponent(i))
        {

            traverse(c);
            
        }

    }

}
