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
#include "HarmonigonPath.h"
#include "TracerPoint.h"

//==============================================================================

#define HEX_W_TO_H_RATIO 1.1547005

class HexGrid : public Component
{
public:
    HexGrid();
    ~HexGrid();

    void paint (Graphics&) override;
    void resized() override;

    void mouseMove(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    HarmonigonPath*  getPath();

    Array<Hexagon*> getNotesToPlay();
    Array<Hexagon*> getFreePlayNotes();
    void advancePaths(int quarterNoteDuration);

    void setSelectionType(bool isHex);
    std::function<void()> onButtonPressed;
    std::function<void()> onButtonReleased;

    void startNewPath(bool isHexPath);
    void endPath();
    void storePath(HarmonigonPath* path);
    void resetPathPositions();

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
    Array <Hexagon*> getNotes(TracerPoint point);
    TracerPoint      getNearestVert(Point<int> pos);
    Array<Hexagon*>  getAdjacentHexes();
    Colour           getNextColour();
    
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
    Colour m_curPathColour;

    int m_numPaths = 0;
    Array<Colour> m_colours;
    OwnedArray<HarmonigonPath> m_paths; /* Completed paths */

    bool m_isHexMode = true; // Tracer or hex select mode for path making

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexGrid)
};