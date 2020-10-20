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
    for(int i = 0; i < pathListItems.size(); i++){
        pathListItems[i]->setBounds(b.removeFromTop(PATH_LIST_ITEM_HEIGHT));
        b.removeFromTop(1);
    }
}

void PathListPanel::addPath(HarmonigonPath* path)
{
    PathListItem* newItem = new PathListItem(path);
    pathListItems.add(newItem);
    addAndMakeVisible(newItem);
    resized();
}

void PathListPanel::deletePathListItem(PathListItem* pathListItem)
{
    pathListItems.removeObject(pathListItem);
    resized();
}

int PathListPanel::getNumPaths()
{
    return pathListItems.size();
}
