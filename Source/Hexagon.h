/*
  ==============================================================================

    Hexagon.h
    Created: 14 Jun 2020 3:32:11pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteUtils.h"

#define Point juce::Point

//==============================================================================
/*
*/
class Hexagon : public Component
{
public:
    Hexagon();
    ~Hexagon();

    void paint (Graphics&) override;
    void resized() override;

    // Vertex index is 0 at top right, goes clockwise
    Point<float> getVertex(int index);
    void setNote(int noteNum);
    void setTile(NoteUtils::HexTile tile);

private:
    int m_noteNum; // MIDI note number
    float m_sideLength;
    Point<float> m_center;
    NoteUtils::HexTile m_tile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hexagon)
};
