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

#define NUM_TRACER_COLS   16
#define NUM_TRACER_ROWS   17
#define SEG_PER_SHORT_COL 14
#define SEG_PER_LONG_COL  16
#define SEG_PER_SHORT_ROW 7
#define SEG_PER_LONG_ROW  8

//==============================================================================
/*
*/
class Tracer : public Component
{
public:
    Tracer();
    ~Tracer();

    void paint (Graphics&) override;
    void resized() override;

    typedef struct _tracerPoint
    {
        _tracerPoint() {}
        _tracerPoint(int pRow, int pCol, int pVertex) : row(pRow), col(pCol), vertex(pVertex) {}
        int row;
        int col;
        int vertex;
    } TracerPoint;
    
    TracerPoint position;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tracer)
};
