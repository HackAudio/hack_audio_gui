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

#ifndef HACK_AUDIO_VIEWPORT_H
#define HACK_AUDIO_VIEWPORT_H

namespace HackAudio
{

/**
 An unbounded, infinitely-scrollable viewport for displaying HackAudio::Diagram components
*/
class Viewport : public juce::Component,
                 private juce::ComponentListener,
                 private juce::ButtonListener
{

public:

    Viewport();
    ~Viewport();

    /**
     Sets the top-level diagram to display, clearing out any traversal chains
    */
    void setDiagram(HackAudio::Diagram& d);

    /**
     Removes any diagrams from the display
    */
    void clearDiagram();

    /**
     Move downwards to a specific diagram, marking the current one as the last parent
    */
    void traverseDown(HackAudio::Diagram& d);

    /**
     Move up to the next parent diagram
    */
    void traverseUp();

    /**
     Return to the top level diagram
    */
    void traverseTop();

    /** 
     Returns true if the diagram is within the traversal chain
    */
    bool isInTraversal(HackAudio::Diagram& d) const;

    /**
     Sets the font to be used when displaying diagram titles
    */
    void setFont(juce::Font newFont);

    /**
     Returns the viewport's current font
    */
    juce::Font getFont() const;

private:

    void setDiagramViaTraversal(HackAudio::Diagram& d);

    void mouseEnter    (const juce::MouseEvent& e) override;
    void mouseExit     (const juce::MouseEvent& e) override;
    void mouseDown     (const juce::MouseEvent& e) override;
    void mouseDrag     (const juce::MouseEvent& e) override;
    void mouseUp       (const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;

    void enablementChanged() override;

    void buttonClicked(juce::Button* b) override;

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    juce::Font viewportFont;

    juce::Component contentContainer;
    HackAudio::Diagram* currentContent;

    juce::Array<HackAudio::Diagram*> parentContent;

    NavigationButton backButton;
    NavigationButton topButton;

    juce::ComponentDragger componentDragger;
    juce::ComponentAnimator componentAnimator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viewport)

};

}

#endif
