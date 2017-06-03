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
