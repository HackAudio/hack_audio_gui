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

namespace HackAudio
{

class Graph : public juce::Component,
              private juce::ComponentListener
{

public:

    class Node : public juce::Component
    {

        friend class Graph;

    public:

        void setAxisLocking(bool shouldLockVertical, bool shouldLockHorizontal);

        void  setXValue(float newX);
        float getXValue() const;

        void  setYValue(float newY);
        float getYValue() const;

        void  setZValue(float newZ);
        float getZValue() const;

    private:
        Node(HackAudio::Graph* graph);

        void mouseEnter    (const juce::MouseEvent& e) override;
        void mouseExit     (const juce::MouseEvent& e) override;
        void mouseDown     (const juce::MouseEvent& e) override;
        void mouseDrag     (const juce::MouseEvent& e) override;

        bool keyPressed(const juce::KeyPress& key) override;

        void focusGained(juce::Component::FocusChangeType cause) override;
        void focusLost  (juce::Component::FocusChangeType cause) override;

        void colourChanged() override;

        void paint(juce::Graphics& g) override;

        bool axisLockedX, axisLockedY;

        float z;

        HackAudio::Graph& owner;
        
    };

    Graph();

    ~Graph();

    void add();
    void add(const juce::String& nodeId);

    void insert(int index);
    void insert(int index, const juce::String& nodeId);

    Graph::Node* getNode(int index) const;
    Graph::Node* getNode(const juce::String& nodeId) const;

    void remove(int index);
    void remove(const juce::String& nodeId);

    void setNodeSize(int newSize);
    int  getNodeSize() const;

    void setGraphPointStatus(bool shouldShowStartAndEndPoints);
    bool getGraphPointStatus() const;

    void setStartPoint(float pos);
    float getStartPoint() const;
    void setEndPoint(float pos);
    float getEndPoint() const;

    void setColourStatus(bool shouldSyncNodeColours);

    struct Listener
    {

        virtual ~Listener() {}

        virtual void graphNodeChanged(Graph*, int nodeIndex, const juce::String& nodeId) = 0;

    };

    void addListener(Graph::Listener* listener);

    void removeListener(Graph::Listener* listener);

private:

    void nodeOrderChanged();
    void nodeChanged(int index, const juce::String& nodeId);

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void enablementChanged() override;
    void colourChanged() override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool coloursSynced;

    bool snapsVertically, snapsHorizontally;

    juce::Component contentContainer;

    juce::ComponentDragger           dragger;
    juce::ComponentBoundsConstrainer constraints;

    int nodeSize;

    float startPoint, endPoint;
    bool  startAndEndShown;

    juce::OwnedArray<Node> graphNodes;

    juce::ListenerList<Graph::Listener> listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph)

};

}
