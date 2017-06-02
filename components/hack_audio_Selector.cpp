HackAudio::Selector::Selector() : selectorJustification(juce::Justification::centred)
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);

    setFont(HackAudio::Fonts::NowRegular.withHeight(DEFAULT_FONTSIZE));

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

void HackAudio::Selector::addItem(const juce::String& newItemText, int itemIndex)
{

    itemIndex = (itemIndex = -1) ? selectorItems.size() : itemIndex;

    selectorItems.insert(itemIndex, newItemText);

    repaint();

}

void HackAudio::Selector::addItemList(const juce::StringArray& items, int firstItemIdOffset)
{

    firstItemIdOffset = (firstItemIdOffset = -1) ? selectorItems.size() : firstItemIdOffset;

    for (int i = 0; i < items.size(); ++i)
    {

        selectorItems.insert(i + firstItemIdOffset, items.strings.getReference(i));

    }

    repaint();

}

void HackAudio::Selector::changeItemText(int itemIndex, const juce::String& newText)
{

    selectorItems[itemIndex] = newText;
    repaint();

}

void HackAudio::Selector::clear(juce::NotificationType notification)
{

    selectorItems.clear();

    if (notification == juce::sendNotification)
    {

    }
    else
    {

    }

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

    setColour(HackAudio::midgroundColourId, HackAudio::Colours::Gray.withMultipliedBrightness(1.15f));
    repaint();

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
