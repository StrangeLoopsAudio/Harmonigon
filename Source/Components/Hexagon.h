/*
  ==============================================================================

    Hexagon.h
    Created: 14 Jun 2020 3:32:11pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Utils/NoteUtils.h"

#define Point juce::Point

using namespace juce;

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
    bool hitTest(int x, int y) override;
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

    void setSelected(Colour colour, bool isFirst);
    void endSelected();
    void setHovering(bool isHovering);
    bool isSelected();
    void clearColor();

private:
    int m_noteNum; // MIDI note number
    Path m_hexPath;
    float m_sideLength;
    Point<float> m_center;
    NoteUtils::HexTile m_tile;
    Colour m_curColour;
    Colour m_pathColour;
    int m_timerCount;
    int m_row, m_col;
    bool m_isHovering = false;
    bool m_isSelected = false;
    bool m_isFirst = false; /* First in path */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hexagon)
};
