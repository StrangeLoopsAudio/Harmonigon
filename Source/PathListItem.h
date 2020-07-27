/*
  ==============================================================================

    PathListItem.h
    Created: 21 Jul 2020 4:43:58pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HarmonigonPath.h"
#include "Hexagon.h"

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
    PathListItem(HarmonigonPath *path);
    ~PathListItem() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    ComboBox repeatType;
    ComboBox stepIntervalType;
    ComboBox loopLength;
    Label name;
    Label repeatTypeLabel;
    Label stepIntervalTypeLabel;
    Label loopLengthLabel;
    
private:
    HarmonigonPath* m_path;

    OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListItem)
};
