/*
  ==============================================================================

    ParameterBar.h
    Created: 5 Jul 2020 8:02:47pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ParameterBar: public Component
{
public:
    ParameterBar();
    ~ParameterBar();

    void paint (Graphics&) override;
    void resized() override;
    
    Slider sliderBpm;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterBar)
};
