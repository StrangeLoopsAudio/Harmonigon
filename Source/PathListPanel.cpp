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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

}

void PathListPanel::resized()
{
    Rectangle<int> b = getLocalBounds();
    for(int i = 0; i < m_pathListItems.size(); i++){
        m_pathListItems[i]->setBounds(b.removeFromTop(PATH_LIST_ITEM_HEIGHT));
        addAndMakeVisible(m_pathListItems[i]);
        /* m_pathListItems[i]->resized(); */
    }
}

void PathListPanel::addPath(PathListItem* p)
{
    m_pathListItems.add(p);
}
