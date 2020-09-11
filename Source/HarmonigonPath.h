/*
  ==============================================================================

    HarmonigonPath.h
    Created: 27 Jul 2020 12:31:42pm
    Author:  brady

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TracerPoint.h"
#include "Hexagon.h"

class HarmonigonPath
{
public:
    HarmonigonPath();
    HarmonigonPath(int id, Colour colour, Array<Hexagon*> hexagons) : id(id), colour(colour), isHexPath(true), hexPath(hexagons), noteIncrement(4), noteIncrementCount(0), notesPlaying(false) { };
    
    HarmonigonPath(int id, Colour colour, Array<TracerPoint*> tracerLinePath) : id(id), colour(colour), isHexPath(false), tracerLinePath(tracerLinePath), noteIncrement(4), noteIncrementCount(0), notesPlaying(false) { };
    
    int id;
    int curIndex = 0;
    Colour colour;
    bool isHexPath; // Boolean for if path is hex or line path
    Array<TracerPoint *> tracerLinePath;
    Array<Hexagon*> hexPath;
    TracerPoint* getCurrentPoint();
    int noteIncrement;
    int noteIncrementCount;
    bool notesPlaying;
//    TracerPoint tracerStart;

    static Array<TracerPoint> getValidNextMoves(Array<TracerPoint*> tracerLinePath);
    Array <TracerPoint*> getTracerLinePath();
    Array <Hexagon*> getHexPath();
};
