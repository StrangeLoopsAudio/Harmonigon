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

class HexGrid : public Component
{
public:
    HexGrid();
    ~HexGrid();

    void paint (Graphics&) override;
    void resized() override;

private:
    Hexagon m_hexArray[NUM_COLS][NUM_ROWS];
    OwnedArray<Tracer> m_tracers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexGrid)
};
