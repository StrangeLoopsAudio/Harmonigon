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
    HarmonigonPath(int id, Colour colour, TracerPoint origin, Array<TracerPoint::Direction> path) : id(id),
        colour(colour), isHexPath(false), tracerStart(origin), pathDirs(path) { };
    HarmonigonPath(int id, Colour colour, Array<Hexagon*> hexagons) : id(id),
        colour(colour), isHexPath(true), selectedHexes(hexagons) { };

    int id;
    int curIndex = 0;
    Colour colour;
    bool isHexPath; // Boolean for if path is hex or line path
    TracerPoint tracerStart;
    Array<TracerPoint::Direction> pathDirs;
    Array<Hexagon*> selectedHexes;
};