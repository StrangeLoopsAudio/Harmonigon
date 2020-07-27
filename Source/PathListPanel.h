/*
  ==============================================================================

    PathListPanel.h
    Created: 21 Jul 2020 4:43:41pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PathListItem.h"

//==============================================================================
/*
*/
class PathListPanel  : public juce::Component
{
public:
    PathListPanel();
    ~PathListPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void addPath(HarmonigonPath* path);
    int getNumPaths();
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListPanel)
    OwnedArray <PathListItem> m_pathListItems;
};
