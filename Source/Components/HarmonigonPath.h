/*
  ==============================================================================

    HarmonigonPath.h
    Created: 27 Jul 2020 12:31:42pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "TracerPoint.h"
#include "Hexagon.h"

using namespace juce;

class HarmonigonPath
{
public:
    HarmonigonPath();
    HarmonigonPath(int id, Colour colour, Array<Hexagon*> hexagons) : id(id), colour(colour), isHexPath(true), hexPath(hexagons), noteIncrement(4), notesPlaying(false) { };

    HarmonigonPath(int id, Colour colour, Array<TracerPoint*> tracerLinePath) : id(id), colour(colour), isHexPath(false), tracerLinePath(tracerLinePath), noteIncrement(4), notesPlaying(false) { };

    int id;
    int curIndex = 0;
    Colour colour;
    bool isHexPath; // Boolean for if path is hex or line path
    Array<TracerPoint *> tracerLinePath;
    bool notesPlaying;
    Array<Hexagon*> hexPath;
    TracerPoint* getCurrentPoint();
    int noteIncrement;

    static Array<TracerPoint> getValidNextMoves(Array<TracerPoint*> tracerLinePath);
    Array <TracerPoint*> getTracerLinePath();
    Array <Hexagon*> getHexPath();
};
