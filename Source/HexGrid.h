/*
  ==============================================================================

    HexGrid.h
    Created: 14 Jun 2020 3:32:44pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Hexagon.h"
#include "Tracer.h"

//==============================================================================

#define HEX_W_TO_H_RATIO 1.1547005

class HexGrid : public Component, Timer
{
public:
    HexGrid();
    ~HexGrid();

    void paint (Graphics&) override;
    void resized() override;
    virtual void timerCallback() override;

private:

    /* Tracer owning rules */
    /*
    - All hexagons own vertices 0, 1
    - Bottom row of odd columns also own vertices 2, 3, 4
    - Top row of odd columns also own vertex 5
    - First column also owns vertices 3, 4
    - Top left hex also owns vertex 5
    - Bottom right hex also owns vertex 2
    */

    TracerPoint getNextVertex(TracerPoint point);
    Array<TracerPoint> getAdjacentVertices(TracerPoint point);
    Point<float> getTracerPosition(TracerPoint point);

    Hexagon m_hexArray[NUM_COLS][NUM_ROWS];
    OwnedArray<Tracer> m_tracers;
    ComponentAnimator m_animator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexGrid)
};
