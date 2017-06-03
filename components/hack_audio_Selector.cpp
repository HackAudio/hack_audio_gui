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

HackAudio::Selector::Selector() : selectorJustification(juce::Justification::centred)
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);

    setFont(HackAudio::Fonts::NowRegular.withHeight(HackAudio::FontHeights::Medium));

    colourInterpolation.reset(50, 0.5);

    selectLeft.buttonDirection  = HackAudio::NavigationButton::Left;
    selectRight.buttonDirection = HackAudio::NavigationButton::Right;

    addAndMakeVisible(selectLeft);
    addAndMakeVisible(selectRight);

    selectLeft.addMouseListener(this, false);
    selectRight.addMouseListener(this, false);

}

HackAudio::Selector::~Selector()
{

}

void HackAudio::Selector::setJustificationType(juce::Justification justification)
{

    selectorJustification = justification;

    repaint();

}

juce::Justification HackAudio::Selector::getJustificationType() const
{

    return selectorJustification;

}

void HackAudio::Selector::add(const juce::String& newItemText, int itemIndex)
{

    itemIndex = (itemIndex = -1) ? selectorItems.size() : itemIndex;

    selectorItems.insert(itemIndex, newItemText);

    repaint();

    itemsChanged();

}

void HackAudio::Selector::add(const char* newItemText, int itemIndex)
{

    itemIndex = (itemIndex = -1) ? selectorItems.size() : itemIndex;

    juce::String charPtr = juce::String::CharPointerType::CharPointer_UTF8(newItemText);
    selectorItems.insert(itemIndex, charPtr);

    repaint();

    itemsChanged();

}

void HackAudio::Selector::addArray(const juce::StringArray& items, int startIndex, int numElementsToAdd)
{

    startIndex = (startIndex > 0) ? selectorItems.size() : startIndex;

    numElementsToAdd = (numElementsToAdd < 0 || numElementsToAdd > items.size()) ? items.size() : numElementsToAdd;

    for (int i = 0; i < numElementsToAdd; ++i)
    {

        selectorItems.insert(i + startIndex, items.strings.getReference(i));

    }

    repaint();

    itemsChanged();

}

void HackAudio::Selector::set(int itemIndex, const juce::String& newText)
{

    selectorItems[itemIndex] = newText;
    repaint();

}

void HackAudio::Selector::set(int itemIndex, const char* newText)
{

    juce::String charPtr = juce::String::CharPointerType::CharPointer_UTF8(newText);
    selectorItems[itemIndex] = charPtr;
    repaint();
    
}

void HackAudio::Selector::clear()
{

    selectorItems.clear();
    itemsChanged();

}

void HackAudio::Selector::setCurrentIndex(int index)
{

    currentIndex = index;
    repaint();

}

int HackAudio::Selector::getCurrentIndex() const
{

    return currentIndex;

}

int HackAudio::Selector::getNumItems() const
{

    return selectorItems.size();

}

juce::String HackAudio::Selector::getCurrentText() const
{

    return selectorItems.getReference(currentIndex);

}

juce::String HackAudio::Selector::getItemText(int index) const
{

    return selectorItems.getReference(index);

}

int HackAudio::Selector::getIndexForItem(const juce::String& itemText) const
{

    for (int i = 0; i < selectorItems.size(); ++i)
    {

        if (selectorItems.getReference(i).compare(itemText) == 0)
        {

            return i;

        }

    }

    return -1;

}

int HackAudio::Selector::getIndexForItem(const char* itemText) const
{

    juce::String charPtr = juce::String::CharPointerType::CharPointer_UTF8(itemText);

    for (int i = 0; i < selectorItems.size(); ++i)
    {

        if (selectorItems.getReference(i).compare(charPtr) == 0)
        {

            return i;

        }
        
    }
    
    return -1;
    
}

void HackAudio::Selector::setFont(juce::Font font)
{
    selectorFont = font;
    resized();
    repaint();
}

juce::Font HackAudio::Selector::getFont() const
{

    return selectorFont;

}

void HackAudio::Selector::addListener(Listener* listener)
{

    if (!listeners.contains(listener))
    {

        listeners.add(listener);

    }

}

void HackAudio::Selector::removeListener(Listener* listener)
{

    listeners.remove(listener);

}

void HackAudio::Selector::indexChanged()
{

    colourInterpolation.setValue(1.0f);
    startTimerHz(ANIMATION_FPS);
    listeners.call(&HackAudio::Selector::Listener::selectorIndexChanged, this, currentIndex);

}

void HackAudio::Selector::itemsChanged()
{

    listeners.call(&HackAudio::Selector::Listener::selectorItemsChanged, this);

}

void HackAudio::Selector::mouseUp(const juce::MouseEvent& e)
{

    if (e.eventComponent == &selectLeft)
    {

        if (currentIndex > 0)
        {

            currentIndex--;
            indexChanged();

        }

    }
    else if (e.eventComponent == &selectRight)
    {

        if (currentIndex < selectorItems.size() - 1)
        {

            currentIndex++;
            indexChanged();

        }

    }

}

bool HackAudio::Selector::keyPressed(const juce::KeyPress& key)
{

    if (key.getKeyCode() == key.escapeKey)
    {

        juce::Component::unfocusAllComponents();
        return true;

    }

    if (key.getKeyCode() == key.leftKey)
    {

        if (currentIndex > 0)
        {

            currentIndex--;
            indexChanged();
            
        }

        return true;
        
    }
    else if (key.getKeyCode() == key.rightKey)
    {

        if (currentIndex < selectorItems.size() - 1)
        {

            currentIndex++;
            indexChanged();
            
        }

        return true;

    }

    return false;

}

void HackAudio::Selector::focusGained(juce::Component::FocusChangeType cause)
{

    if (cause == juce::Component::focusChangedByTabKey)
    {

        setColour(HackAudio::midgroundColourId, HackAudio::Colours::LightGray);
        repaint();

    }

}

void HackAudio::Selector::focusLost(juce::Component::FocusChangeType cause)
{

    setColour(HackAudio::midgroundColourId, HackAudio::Colours::Gray);
    repaint();

}

void HackAudio::Selector::enablementChanged()
{

    selectLeft.setEnabled(isEnabled());
    selectRight.setEnabled(isEnabled());

    if (isEnabled())
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
        setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
        setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    }
    else
    {

        setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
        setColour(HackAudio::foregroundColourId, HackAudio::Colours::Black);
        setColour(HackAudio::highlightColourId,  HackAudio::Colours::Black);
        
    }
    
    repaint();
    
}

void HackAudio::Selector::timerCallback()
{

    if (colourInterpolation.isSmoothing())
    {

        repaint();

        if (std::abs(colourInterpolation.getTargetValue() - colourInterpolation.getNextValue()) < 0.0001)
        {
            colourInterpolation.setValue(colourInterpolation.getTargetValue());
        }

    }
    else
    {

        if (colourInterpolation.getTargetValue() == 1.0f)
        {
            colourInterpolation.setValue(0.0f);
        }
        else
        {

            repaint();
            stopTimer();
            
        }
        
    }

}

void HackAudio::Selector::paint(juce::Graphics& g)
{

    int width = getWidth();
    int height = getHeight();

    juce::Path p;
    p.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
    g.setColour(findColour(HackAudio::midgroundColourId));
    g.fillPath(p);

    juce::Colour foreground = findColour(HackAudio::foregroundColourId);
    juce::Colour highlight  = findColour(HackAudio::highlightColourId);

    g.setColour(foreground.interpolatedWith(highlight, colourInterpolation.getNextValue()));
    g.setFont(getFont());
    g.drawFittedText(selectorItems[currentIndex], 0, 0, width, height, selectorJustification.getFlags(), 1);

}

void HackAudio::Selector::resized()
{

    int width  = getWidth();
    int height = getHeight();

    selectLeft.setBounds(8, height/2 - 8, 16, 16);
    selectRight.setBounds(width - 24, height/2 - 8, 16, 16);

}
