/*
  ==============================================================================

    HexGrid.cpp
    Created: 14 Jun 2020 3:32:44pm
    Author:  brady

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HexGrid.h"

//==============================================================================
HexGrid::HexGrid()
{
    setSize(200, 300);
    for (int i = 0; i < NUM_COLS; i++)
    {
        int numRows = (i % 2 == 0) ? (NUM_ROWS - 1) : NUM_ROWS;

        for (int j = 0; j < numRows; j++)
        {
            /* Yes this is bad for locality, but it's the easiest to traverse with flat hexagons */
            m_hexArray[i][j].setTile(NoteUtils::hexagons[j][i]);
            addAndMakeVisible(m_hexArray[i][j]);
        }
    }
    // Add a tracerboi
    m_tracers.add(new Tracer());
    m_tracers[0]->setSize(10, 10);
    m_tracers[0]->position = Tracer::TracerPoint(3, 3, 0);
    addAndMakeVisible(m_tracers[0]);

    startTimer(1000);
}

HexGrid::~HexGrid()
{
}

void HexGrid::timerCallback()
{
    if (!m_animator.isAnimating())
    {
        Rectangle<int> center = m_tracers[0]->getBounds();
        Tracer::TracerPoint newPoint = m_tracers[0]->position;
        newPoint.row++;
        center.setCentre(getTracerPosition(newPoint).toInt());
        m_animator.animateComponent(m_tracers[0], center, 1, 900, false, 0.3, 0.3);
    }   
}

void HexGrid::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void HexGrid::resized()
{
    /* Position hexagons */
    float hexHeight = getHeight() / NUM_ROWS;
    float hexWidth = hexHeight * HEX_W_TO_H_RATIO;

    float curX = 0;
    float curY = 0;
    float yOffset = hexHeight / 2;
    int numRows = NUM_ROWS;
    for (int i = 0; i < NUM_COLS; i++)
    {
        if (i > 0)
        {
            // Set x to previous column's top right vertex coordinate
            curX = m_hexArray[i-1][0].getVertex(0).x;
        }
        curY = (i % 2 == 0) ? yOffset : 0;
        numRows = (i % 2 == 0) ? (NUM_ROWS - 1) : NUM_ROWS;

        for (int j = 0; j < numRows; j++)
        {
            /* Yes this is bad for locality, but it's the easiest to traverse with flat hexagons */
            m_hexArray[i][j].setBounds(curX, curY, hexWidth, hexHeight);
            curY += hexHeight;
        }
    }

    for (int i = 0; i < m_tracers.size(); i++)
    {
        Point<float> tracerPos = getTracerPosition(m_tracers[i]->position);
        m_tracers[i]->setCentrePosition(tracerPos.toInt());
    }
}

Point<float> HexGrid::getTracerPosition(Tracer::TracerPoint point)
{
    return m_hexArray[point.col][point.row].getVertex(point.vertex);
}