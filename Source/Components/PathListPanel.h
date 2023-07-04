/*
  ==============================================================================

    PathListPanel.h
    Created: 21 Jul 2020 4:43:41pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PathListItem.h"

using namespace juce;

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
//    void buttonClicked(Button* button) override;

    void addPath(HarmonigonPath* path);
    void deletePathListItem(PathListItem* pathListItem);
    int getNumPaths();

    OwnedArray <PathListItem> pathListItems;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListPanel)
};
