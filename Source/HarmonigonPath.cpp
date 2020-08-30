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
    DBG("path size = " << tracerLinePath.size());
    Array<TracerPoint::Direction> allMoves = tracerLinePath.getLast()->getMoves();
    Array<TracerPoint> validPoints;
//    Array<TracerPoint> allPoints; // all points in tracerLinePath parameter
//    allPoints.add(*this);
//    TracerPoint point = *tracerLinePath.getLast();
//    for (TracerPoint* tracerPoint : tracerLinePath)
//    {
//        point.move(dir);
//        allPoints.add(tracerPoint);
//    }
    bool repeatPointFound = false;

    /* Point should be at end of path, check if hit any of the same points */
    for (int i = 0; i < allMoves.size(); i++)
    {
        repeatPointFound = false;
        TracerPoint newPoint = *tracerLinePath.getLast();
        newPoint.move(allMoves[i]);
//        newPoint.pos.col = tracerLinePath.getLast()->pos.col;
//        newPoint.pos.row = tracerLinePath.getLast()->pos.row;
//        newPoint.vertex = tracerLinePath.getLast()->vertex;
//        newPoint.intType = tracerLinePath.getLast()->intType;

//        TracerPoint* newPoint = tracerLinePath.getLast();
        
//        DBG("old row " << newPoint->pos.row);
//        DBG("old col " << newPoint->pos.col);
//        newPoint = tracerLinePath.getLast();
        
//        newPoint.move(allMoves[i]);
        
//        DBG("new row " << newPoint->pos.row);
//        DBG("new col " << newPoint->pos.col);

        for (TracerPoint* pointInPath : tracerLinePath)
        {
            if (*pointInPath == newPoint)
            {
//                allMoves.remove(i);
                DBG("repeat found");
                repeatPointFound = true;
//                allPoints.remove(<#int indexToRemove#>);
            }
        }
        if(!repeatPointFound)
        {
            validPoints.add(newPoint);
        }
    }
    DBG("Valid Points: ");
    for(TracerPoint point : validPoints)
    {
        DBG("line row = " << point.pos.row);
        DBG("line col = " << point.pos.col);

    }
    DBG(" ");
    return validPoints;
}
