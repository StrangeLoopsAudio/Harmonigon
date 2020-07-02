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
int timerCount = 0;

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

    /*m_tracers.add(new Tracer());
    m_tracers[0]->setSize(15, 15);
    m_tracers[0]->position = TracerPoint(7, 13, 4);
    addAndMakeVisible(m_tracers[0]); */
    
    for (int i = 0; i < 10; i++)
    {
        m_tracers.add(new Tracer());
        m_tracers[i]->setSize(15, 15);
        m_tracers[i]->position = TracerPoint(0, 0, 1);
        addAndMakeVisible(m_tracers[i]);
    }

    startTimer(500);
}

HexGrid::~HexGrid()
{
}

void HexGrid::timerCallback()
{
    if (!m_animator.isAnimating())
    {
        timerCount++;
        for (int i = 0; i < m_tracers.size(); i++){
//            if (timerCount % 4 < i % 10)
//            {
                Rectangle<int> center = m_tracers[i]->getBounds();
                moveTracerRandom(m_tracers[i]);
                center.setCentre(getTracerPosition(m_tracers[i]->position).toInt());
                m_animator.animateComponent(m_tracers[i], center, 1, 400, true, 0.3, 0.3);
                repaint();
//            }
        }
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

Point<float> HexGrid::getTracerPosition(TracerPoint point)
{
    return m_hexArray[point.hexPos.col][point.hexPos.row].getVertex(point.vertex);
}

void HexGrid::moveTracerRandom(Tracer *tracer)
{
    Array<TracerPoint::Direction> possibleDirs = tracer->position.getMoves();
    int index = Random::getSystemRandom().nextInt(possibleDirs.size());
    /* DBG("possibleDirs");
    for(int i = 0; i < possibleDirs.size(); i++){
        DBG(possibleDirs[i]);
    }
    DBG("actual = " << possibleDirs[index] << "\n"); */
    return tracer->position.move(possibleDirs[index]);
}
