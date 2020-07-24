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

class HexGrid : public Component, public MouseListener
{
public:
    HexGrid();
    ~HexGrid();

    void paint (Graphics&) override;
    void resized() override;

    void mouseMove(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;

    Array<Hexagon*>  getHexPath();
    TracerPoint      getLinePathOrigin();
    Array<TracerPoint::Direction> getLinePath();

    Array<Hexagon*> getNotesToPlay();
    void moveTracers(int duration);

    void addPathClicked(bool isAdding);
    void pathModeChanged(bool isInHexMode);

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

    void             moveTracerRandom(Tracer *tracer);
    Point<float>     getTracerPosition(TracerPoint point);
    Hexagon*         getTracerHex(Tracer* tracer);
    Array <Hexagon*> getNotes(Tracer *tracer);
    TracerPoint      getNearestVert(Point<int> pos);
    
    Hexagon m_hexArray[NUM_COLS][NUM_ROWS];
    OwnedArray<Tracer> m_tracers;
    ComponentAnimator m_animator;
    int m_timerCount = 0;

    /* Path adding vars */
    bool m_canSelect = false;
    TracerPoint m_hoveringOverPoint;
    Hexagon* m_hoveringOverHex = nullptr;
    TracerPoint m_tracerStart;
    Array<TracerPoint::Direction> m_pathDirs;
    Array<Hexagon*> m_selectedHexes;

    bool m_isHexMode = true; // Tracer or hex select mode for path making

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexGrid)
};
