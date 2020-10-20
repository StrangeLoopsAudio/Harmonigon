/*
  ==============================================================================

    HarmonigonPath.cpp
    Created: 27 Jul 2020 12:31:42pm
    Author:  brady

  ==============================================================================
*/

#include "HarmonigonPath.h"

Array<TracerPoint> HarmonigonPath::getValidNextMoves(Array<TracerPoint*> tracerLinePath)
{
    Array<TracerPoint::Direction> allMoves = tracerLinePath.getLast()->getMoves();
    Array<TracerPoint> validPoints;

    bool repeatPointFound = false;

    /* Point should be at end of path, check if hit any of the same points */
    for (int i = 0; i < allMoves.size(); i++)
    {
        repeatPointFound = false;
        TracerPoint newPoint = *tracerLinePath.getLast();
        newPoint.move(allMoves[i]);

        for (TracerPoint* pointInPath : tracerLinePath)
        {
            if (*pointInPath == newPoint)
            {
                repeatPointFound = true;
            }
        }

        if(!repeatPointFound)
        {
            validPoints.add(newPoint);
        }
    }

    return validPoints;
}

Array <TracerPoint*> HarmonigonPath::getTracerLinePath()
{
    return tracerLinePath;
}

Array <Hexagon*> HarmonigonPath::getHexPath()
{
    return hexPath;
}

TracerPoint* HarmonigonPath::getCurrentPoint()
{
    jassert(!isHexPath);
    return tracerLinePath[curIndex];
}
