/*
  ==============================================================================

    TracerPanelObject.h
    Created: 19 Jul 2020 9:50:47pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TracerPanelObject  : public juce::Component
{
public:
    TracerPanelObject();
    ~TracerPanelObject() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracerPanelObject)
};
