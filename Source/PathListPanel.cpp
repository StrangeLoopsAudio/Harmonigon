/*
  ==============================================================================

    PathListPanel.cpp
    Created: 21 Jul 2020 4:43:41pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PathListPanel.h"

#define PATH_LIST_ITEM_HEIGHT 100
//==============================================================================
PathListPanel::PathListPanel()
{
    setSize(300, 700);

    m_colours =
    {
        Colours::crimson,
        Colours::goldenrod,
        Colours::palegreen,
        Colours::orchid,
        Colours::peachpuff,
        Colours::lavender
    };
}

PathListPanel::~PathListPanel()
{
}

void PathListPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PathListPanel::resized()
{
    Rectangle<int> b = getLocalBounds();
    for(int i = 0; i < m_pathListItems.size(); i++){
        m_pathListItems[i]->setBounds(b.removeFromTop(PATH_LIST_ITEM_HEIGHT));
    }
}

Colour PathListPanel::getNextColour()
{
    return m_colours[m_colourIndex % m_colours.size()];
}

void PathListPanel::addPath(TracerPoint origin, Array<TracerPoint::Direction> path)
{
    PathListItem* newItem = new PathListItem(m_pathListItems.size(), m_colours[m_colourIndex], origin, path);
    m_pathListItems.add(newItem);
    addAndMakeVisible(newItem);
    m_colourIndex++;
    resized();
}

void PathListPanel::addPath(Array<Hexagon*> hexagons)
{
    PathListItem* newItem = new PathListItem(m_pathListItems.size(), m_colours[m_colourIndex], hexagons);
    m_pathListItems.add(newItem);
    addAndMakeVisible(newItem);
    m_colourIndex++;
    resized();
}

int PathListPanel::getNumPaths()
{
    return m_pathListItems.size();
}
