/*
  ==============================================================================

    ParameterBar.h
    Created: 5 Jul 2020 8:02:47pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "NoteUtils.h"

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
    ComboBox comboKey;
    ComboBox comboScaleType;
    TextButton buttonAddPath;
    TextButton buttonPathMode;
    TextButton buttonPlayStop;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterBar)
};
