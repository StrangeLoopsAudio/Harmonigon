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
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    Font getComboBoxFont(ComboBox &) override;
};

class PathListItem  : public juce::Component
{
public:
    PathListItem(int id);
    ~PathListItem() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    ComboBox repeatType;
    ComboBox stepIntervalType;
    ComboBox loopLength;
    Label name;
    
private:
    int id;
    OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListItem)
};
