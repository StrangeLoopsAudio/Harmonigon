/*
  ==============================================================================

    PathListPanel.h
    Created: 21 Jul 2020 4:43:41pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListPanel)
};
