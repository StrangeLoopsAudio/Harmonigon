/*
  ==============================================================================

    PathListItem.h
    Created: 21 Jul 2020 4:43:58pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "HarmonigonPath.h"
#include "Hexagon.h"
#include "HexGrid.h"

using namespace juce;

//==============================================================================
/*
*/
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    Font getComboBoxFont(ComboBox &) override;
};

class PathListItem  : public juce::Component, ComboBox::Listener
{
public:
    PathListItem(HarmonigonPath *path);
    ~PathListItem() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    HarmonigonPath* getPath();

    ComboBox repeatType;
    ComboBox stepIntervalType;
    ComboBox loopLength;
    Label name;
    Label repeatTypeLabel;
    Label stepIntervalTypeLabel;
    Label loopLengthLabel;
    TextButton buttonDeletePath;
    bool buttonListenerAdded = false;

private:
    HarmonigonPath* m_path;
    OtherLookAndFeel otherLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListItem)
};
