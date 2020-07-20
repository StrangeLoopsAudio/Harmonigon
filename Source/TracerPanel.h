/*
  ==============================================================================

    TracerPanel.h
    Created: 19 Jul 2020 9:36:19pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TracerPanel  : public juce::Component
{
public:
    TracerPanel();
    ~TracerPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracerPanel)
};
