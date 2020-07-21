/*
  ==============================================================================

    PathListItem.h
    Created: 21 Jul 2020 4:43:58pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PathListItem  : public juce::Component
{
public:
    PathListItem();
    ~PathListItem() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListItem)
};
