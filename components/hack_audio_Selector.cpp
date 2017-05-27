HackAudio::Selector::Selector() : selectorJustification(juce::Justification::centred)
{

    setColour(HackAudio::backgroundColourId, HackAudio::Colours::Black);
    setColour(HackAudio::midgroundColourId,  HackAudio::Colours::Gray);
    setColour(HackAudio::foregroundColourId, HackAudio::Colours::White);
    setColour(HackAudio::highlightColourId,  HackAudio::Colours::Cyan);

    setMouseClickGrabsKeyboardFocus(true);

    setFont(HackAudio::Fonts::Now.withHeight(18));

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

juce::Justification HackAudio::Selector::getJustificationType()
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

int HackAudio::Selector::getCurrentIndex()
{

    return currentIndex;

}

int HackAudio::Selector::getNumItems()
{

    return selectorItems.size();

}

juce::String HackAudio::Selector::getCurrentText()
{

    return selectorItems.getReference(currentIndex);

}

juce::String HackAudio::Selector::getItemText(int index)
{

    return selectorItems.getReference(index);

}

int HackAudio::Selector::getIndexForItem(const juce::String& itemText)
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

juce::Font HackAudio::Selector::getFont()
{

    return selectorFont;

}

void HackAudio::Selector::mouseUp(const juce::MouseEvent& e)
{

    if (e.eventComponent == &selectLeft)
    {

        currentIndex -= (currentIndex > 0) ? 1 : 0;
        colourInterpolation.setValue(1.0f);
        startTimerHz(ANIMATION_FPS);

    }
    else if (e.eventComponent == &selectRight)
    {

        currentIndex += (currentIndex < selectorItems.size() - 1) ? 1 : 0;
        colourInterpolation.setValue(1.0f);
        startTimerHz(ANIMATION_FPS);

    }

}

bool HackAudio::Selector::keyPressed(const juce::KeyPress& key)
{

    if (key.getKeyCode() == key.leftKey)
    {

        currentIndex -= (currentIndex > 0) ? 1 : 0;
        repaint();
        return true;
        
    }
    else if (key.getKeyCode() == key.rightKey)
    {

        currentIndex += (currentIndex < selectorItems.size()) ? 1 : 0;
        repaint();
        return true;

    }

    return false;

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
    p.addRoundedRectangle(0, 0, width, height, CORNER_RADIUS, CORNER_CONFIG);
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

    selectLeft.setBounds(height / 4, height / 4, height / 2, height / 2);
    selectRight.setBounds(width - (height - (height / 4)), height / 4, height / 2, height / 2);

}
