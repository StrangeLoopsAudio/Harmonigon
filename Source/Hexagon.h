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
class Hexagon : public Component, Timer
{
public:
    Hexagon();
    ~Hexagon();

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

    // Vertex index is 0 at top right, goes clockwise
    Point<float> getVertex(int index);
    void setNote(int noteNum);
    void setTile(NoteUtils::HexTile tile);
    void setPosition(int row, int col); /* Needed for mouse listeners */
    int getRow();
    int getCol();
    void pulse(); /* Flash a note when it's played */
    NoteUtils::HexTile getTile();
    bool isPointInside(Point<float> point); /* Checks if a point is in the hexagon path in this component */

    void setSelected(bool isSelected);
    
private:
    int m_noteNum; // MIDI note number
    Path m_hexPath;
    float m_sideLength;
    Point<float> m_center;
    NoteUtils::HexTile m_tile;
    Colour m_curColour;
    int m_timerCount;
    int m_row, m_col;
    bool m_isSelected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hexagon)
};
