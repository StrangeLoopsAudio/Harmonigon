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

struct HarmonigonPath
{
    HarmonigonPath();
    HarmonigonPath(int id, Colour colour, TracerPoint origin, Array<TracerPoint::Direction> path) : id(id),
        colour(colour), isHexPath(false), tracerStart(origin), pathDirs(path) { };
    HarmonigonPath(int id, Colour colour, Array<Hexagon*> hexagons) : id(id),
        colour(colour), isHexPath(true), selectedHexes(hexagons) { };
    
    int id;
    Colour colour;
    bool isHexPath; // Boolean for if path is hex or line path
    TracerPoint tracerStart;
    Array<TracerPoint::Direction> pathDirs;
    Array<Hexagon*> selectedHexes;
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
