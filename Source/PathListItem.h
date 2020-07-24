/*
  ==============================================================================

    PathListItem.h
    Created: 21 Jul 2020 4:43:58pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Tracer.h"
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
    PathListItem(int id, Colour colour, TracerPoint origin, Array<TracerPoint::Direction> path);
    PathListItem(int id, Colour colour, Array<Hexagon*> hexagons);
    ~PathListItem() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void initializeItem();

    ComboBox repeatType;
    ComboBox stepIntervalType;
    ComboBox loopLength;
    Label name;
    Label repeatTypeLabel;
    Label stepIntervalTypeLabel;
    Label loopLengthLabel;
    
private:
    int m_id;
    bool m_isHexPath; // Boolean for if path is hex or line path
    TracerPoint m_tracerStart;
    Array<TracerPoint::Direction> m_pathDirs;
    Array<Hexagon*> m_selectedHexes;
    Colour m_pathColour;

    OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathListItem)
};
