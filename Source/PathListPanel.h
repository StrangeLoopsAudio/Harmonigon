/*
  ==============================================================================

    PathListPanel.h
    Created: 19 Jul 2020 9:36:19pm
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
        
private:
    OwnedArray<PathListItem> m_pathListItems;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListPanel)


};
