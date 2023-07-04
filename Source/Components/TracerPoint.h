/*
  ==============================================================================

    TracerPoint.h
    Created: 27 Jul 2020 12:31:18pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TracerPoint
{
public:
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
    TracerPoint getPathEnd(Array<Direction> path); /* Assuming start is the current tracer, return the point at the end of the path */
    void move(Direction dir);
    bool operator==(TracerPoint const& point);
    bool operator!=(TracerPoint const& point);

    coord hexPos;
    coord pos;
    int vertex;
    IntersectionType intType;
};
