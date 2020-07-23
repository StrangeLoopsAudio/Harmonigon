/*
  ==============================================================================

    Tracer.h
    Created: 14 Jun 2020 3:32:20pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Hexagon.h"

//==============================================================================
/*
*/
struct TracerPoint
{    
    enum IntersectionType
    {
        INVALID = -1,
        LEFT_T,
        RIGHT_T,
        UP_DOWN,
        LEFT_RIGHT,
    };

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    TracerPoint() : hexPos(0, 0), vertex(0), intType(INVALID) {}
    TracerPoint(int row, int col, int vertex, bool isHex) : vertex(vertex), intType(INVALID)
    {
        if (isHex)
        {
            hexPos = coord(row, col);
            initLinePos();
        }
        else
        {
            pos = coord(row, col);
            positionChanged();
        }
    }

    typedef struct _coord
    {
        _coord() {}
        _coord(int pRow, int pCol) : row(pRow), col(pCol) {}
        int row;
        int col;
    } coord;
    
    void initLinePos();
    void positionChanged();
    Array<Direction> getMoves();
    void move(Direction dir);
    bool operator==(TracerPoint const& point);

    coord hexPos;
    coord pos;
    int vertex;
    IntersectionType intType;
};

class Tracer : public Component
{
public:
    Tracer();
    ~Tracer();

    void paint (Graphics&) override;
    void resized() override;
    
    TracerPoint position;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tracer)
};
