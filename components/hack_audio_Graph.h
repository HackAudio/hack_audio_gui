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
/**
 A component for using control points to manipulate a curve function
*/
class Graph : public juce::Component,
              private juce::ComponentListener
{

public:

    /**
     A control point used for HackAudio::Graphs. This class should not be 
     instantiated directly, instead use the relevant HackAudio::Graph methods 
     to control Node adding and removal.
     
     @see HackAudio::Graph::add, HackAudio::Graph::insert, HackAudio::Graph::remove
    */
    class Node : public juce::Component,
                 public juce::SettableTooltipClient
    {

        friend class Graph;

    public:

        /**
         Restricts whether or not a Node should be able to move along a certain axis.
         
         Note that the two arguments cannot both be true, instead use the graph's 
         enablement to set whether or not the user can interact with the graph
         
         @param shouldLockVertical      whether or not to lock Y-axis movement
         @param shouldLockHorizontal    whether or not to lock X-axis movement
        */
        void setAxisLocking(bool shouldLockVertical, bool shouldLockHorizontal);

        /**
         Whether or not to automatically set the node's tooltip to its XY values.
         
         If this is enabled, the tooltip will be overwritten every time
         the node is moved or hovered over.

         If this is disabled, you can set a custom tooltip for the node that will
         be preserved. When disabling this mode, the tooltip is cleared.
         
         This value defaults to true when a node is created.
         
         @param shouldDisplayValues     whether or not to automatically update the tooltip
        */
        void setValueDisplay(bool shouldDisplayValues);

        /**
         Sets the node's X value. This is similar to its X position, but on a range
         of 0.0 - 1.0 along the width of the graph itself.
         
         @param newX    a float ratio from 0.0 - 1.0
        */
        void  setXValue(float newX);

        /**
         Returns the current value of the node's X position with respect to 
         the graph's width
        */
        float getXValue() const;

        /**
         Sets the node's Y value. This is similar to its Y position, but on a range
         of 0.0 - 1.0 along the height of the graph itself.

         @param newY    a float ratio from 0.0 - 1.0
        */
        void  setYValue(float newY);

        /**
         Returns the current value of the node's Y position with respect to
         the graph's height
        */
        float getYValue() const;

        /**
         Sets the node's Z value. This value is an arbitrary third dimension
         used by the graph's interpolation functions. Users can set their own
         interpolation functions that utilize this value in any way they like
         
         @param newZ    a float representing the Z value (no current range restriction)
        */
        void  setZValue(float newZ);

        /**
         Returns the Node's current Z value
        */
        float getZValue() const;

        /**
         Returns the Node's centre position relevant to its parent
        */
        juce::Point<int> getNodePosition() const;

    private:
        Node(HackAudio::Graph* graph);

        void updateTooltip();

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

        bool displayValues;

        float z;

        HackAudio::Graph& owner;
        
    };

    Graph();
    ~Graph();

    /**
     Adds a new, unidentified node to the graph
    */
    Graph::Node* add();

    /**
     Adds a new node to the graph and assigns it a ComponentID for easy referencing
     
     @param nodeId      a string to use as the ComponentID
    */
    Graph::Node* add(const juce::String& nodeId);

    /**
     Inserts a new, unidentified node to the graph at the index argument
     
     @param index       where to insert the new node
    */
    Graph::Node* insert(int index);

    /**
     Inserts a new node to the graph and assigns it a ComponentID

     @param index       where to insert the new node
     @param nodeId      the ComponentID to give the node
    */
    Graph::Node* insert(int index, const juce::String& nodeId);

    /**
     Returns a node based on the node's position in which it was added
    */
    Graph::Node* getNode(int index) const;

    /**
     Returns the node with the given ComponentID
     
     @param nodeId      the ComponentID to search for
    */
    Graph::Node* getNode(const juce::String& nodeId) const;

    /**
     Removes the node at a given index from the graph
     
     @param index       the index of the node to remove
    */
    void remove(int index);

    /**
     Removes the node with the matching ComponentID from the graph
     
     @param nodeId      the ComponentID of the node to remove
    */
    void remove(const juce::String& nodeId);

    /**
     Sets the size at which to draw the graph nodes
    */
    void setNodeSize(int newSize);

    /**
     Returns the size at which the graph nodes are drawn
    */
    int getNodeSize() const;

    /**
     Sets the line width to use when drawing paths between nodes
     
     This defaults to 4.0f
    */
    void setLineWidth(float width);

    /**
     Returns the default line width that was set
    */
    float getLineWidth() const;

    /**
     Sets whether or not the graph has start and end node connections along its sides
    */
    void setGraphPointStatus(bool shouldShowStartAndEndPoints);

    /**
     Returns whether or not the graph displays its start and end nodes along its sides
    */
    bool getGraphPointStatus() const;

    /**
     Sets the relative Y position at which to draw the start node
     
     @param pos     a float ratio from 0.0 - 1.0
    */
    void setStartPoint(float pos);

    /**
     Returns the relative Y position the start point is drawn at
    */
    float getStartPoint() const;

    /**
     Sets the relative Y position at which to draw the end node
     
     @param pos     a float ratio from 0.0 - 1.0
    */
    void setEndPoint(float pos);

    /**
     Returns the relative Y position the end point is drawn at
    */
    float getEndPoint() const;

    /**
     Sets the numerical range for the X axis
    */
    void setXRange(int min, int max);
    void setXRange(float min, float max);
    void setXRange(double min, double max);

    /**
     Returns a juce::var containing the minimum X value that was set
    */
    juce::var getXMin() const;

    /**
     Returns a juce::var of the maximum X value that was set
    */
    juce::var getXMax() const;

    /**
     Sets the unit suffix to display for the X axis
    */
    void setXUnits(const juce::String &units);

    /**
     Returns the unit suffix for the X axis
    */
    juce::String getXUnits() const;

    /**
     Sets the numerical range for the Y axis
    */
    void setYRange(int min, int max);
    void setYRange(float min, float max);
    void setYRange(double min, double max);

    /**
     Returns a juce::var of the minimum Y value that was set
    */
    juce::var getYMin() const;

    /**
     Returns a juce::var of the maximum Y value that was set
    */
    juce::var getYMax() const;

    /**
     Sets the unit suffix to display for the Y axis
    */
    void setYUnits(const juce::String &units);

    /**
     Returns the unit suffix for the Y axis
    */
    juce::String getYUnits() const;

    /**
     Sets whether all nodes should share the same colour or not.
     If this is disabled, the user can manually colour nodes using different
     colour values
    */
    void setColourStatus(bool shouldSyncNodeColours);

    /**
     Override to draw on the graph without using nodes
     
     This is only called if no nodes have been set
    */
    virtual juce::Path drawGraph(juce::Rectangle<int> graphBounds, juce::Point<int> start, juce::Point<int> end);

    /**
     Override this method if you wish to add special graphics to the graph's background,
     such as gridlines
     
     All graphics must be drawn within the graphBounds rectangle
    */
    virtual void drawGraphBackground(juce::Graphics &g, juce::Rectangle<int> graphBounds);

    /**
     Override this function to create your own interpolation curves between nodes
    */
    virtual juce::Path drawLineBetween(Graph::Node* nodeOne, Graph::Node* nodeTwo);

    /**
     Override this function to create your own interpolation for the graph's starting point (useful for equalizers)
    */
    virtual juce::Path drawLineFromStart(juce::Point<int> graphStart, Graph::Node* firstNode);

    /**
     Override this function to create your own interpolation for the graph's ending point (useful for equalizers)
    */
    virtual juce::Path drawLineToEnd(Graph::Node* lastNode, juce::Point<int> graphEnd);

    /**
     A class for receiving callbacks when a graph's nodes are moved or changed
    */
    struct Listener
    {

        virtual ~Listener() {}

        /**
         Called when a graph's node has its X, Y, or Z values changed
        */
        virtual void graphNodeChanged(Graph*, Graph::Node*) = 0;

    };

    /**
     Adds a listener to the graph
    */
    void addListener(Graph::Listener* listener);

    /**
     Removes a previously registered listener from the graph
    */
    void removeListener(Graph::Listener* listener);

private:

    void nodeOrderChanged();
    void nodeChanged(Graph::Node* n);

    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;

    void enablementChanged() override;
    void colourChanged() override;

    void mouseUp(const juce::MouseEvent &e) override;

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;

    void resized() override;

    bool coloursSynced;

    bool snapsVertically, snapsHorizontally;

    juce::Component contentContainer;

    juce::ComponentDragger           dragger;
    juce::ComponentBoundsConstrainer constraints;

    int nodeSize;

    float lineWidth;

    float startPoint, endPoint;
    bool  startAndEndShown;

    juce::var xmin, xmax;
    juce::var ymin, ymax;

    juce::String xUnits;
    juce::String yUnits;

    juce::OwnedArray<Node> graphNodes;

    juce::ListenerList<Graph::Listener> listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph)

};

}
