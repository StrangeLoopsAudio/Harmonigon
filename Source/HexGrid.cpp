/*
  ==============================================================================

    HexGrid.cpp
    Created: 14 Jun 2020 3:32:44pm
    Author:  brady

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HexGrid.h"

#define PADDING 15

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
            m_hexArray[i][j].setPosition(j, i);
            m_hexArray[i][j].addMouseListener(this, false);
            addAndMakeVisible(m_hexArray[i][j]);
        }
    }
    
    /* for (int i = 0; i < 2; i++)
    {
        m_tracers.add(new Tracer());
        m_tracers[i]->setSize(15, 15);
        m_tracers[i]->position = TracerPoint(5, 7, 0);
        addAndMakeVisible(m_tracers[i]);
    } */

    m_timerCount = 0;    
}

void HexGrid::addPathClicked(bool isAdding)
{
    m_canSelect = isAdding;
}

void HexGrid::pathModeChanged(bool isInHexMode)
{
    m_isHexMode = isInHexMode;
}

void HexGrid::mouseMove(const MouseEvent& e)
{
    if (m_canSelect)
    {
        if (m_isHexMode)
        {
            Hexagon* hex = (Hexagon*)e.eventComponent;
            if (hex != m_hoveringOverHex)
            {
                if (m_hoveringOverHex != nullptr)
                {
                    m_hoveringOverHex->setHovering(false);
                }
                m_hoveringOverHex = hex;
                hex->setHovering(true);
            }
        }
        else
        {
            m_hoveringOverPoint = getNearestVert(getLocalPoint(e.eventComponent, e.getMouseDownPosition()));
        }

        repaint();
    }
}

void HexGrid::mouseExit(const MouseEvent& event)
{
    if (m_canSelect)
    {
        if (m_isHexMode)
        {
            m_hoveringOverHex->setHovering(false);
            m_hoveringOverHex = nullptr;
        }
        else
        {
            /* Move offscreen if mouse is off of grid */
            m_hoveringOverPoint = TracerPoint();
            repaint();
        }
    }
}

void HexGrid::mouseDown(const MouseEvent& event)
{
    if (m_canSelect)
    {
        if (m_isHexMode && m_hoveringOverHex != nullptr)
        {
            m_selectedHexes.add(m_hoveringOverHex);
            m_hoveringOverHex->setSelected(true);
        }
        else if (!m_isHexMode)
        {
            if (m_hoveringOverPoint.intType != TracerPoint::INVALID)
            {
                m_tracerStart = m_hoveringOverPoint;
            }
        }
        
    }
}

void HexGrid::moveTracers(int duration)
{
    if (!m_animator.isAnimating())
    {
        m_timerCount++;
        for (int i = 0; i < m_tracers.size(); i++)
        {
            Rectangle<int> center = m_tracers[i]->getBounds();
            moveTracerRandom(m_tracers[i]);
            center.setCentre(getTracerPosition(m_tracers[i]->position).toInt());
            m_animator.animateComponent(m_tracers[i], center, 1, duration - 10, true, 0.3, 0.3);
            repaint();
        }
    }
}

HexGrid::~HexGrid()
{
}

void HexGrid::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background
    if (m_hoveringOverPoint.intType != TracerPoint::INVALID)
    {
        g.setColour(Colours::aqua);
        Rectangle<float> circle(0, 0, 15, 15);
        Point<float> vert = m_hexArray[m_hoveringOverPoint.hexPos.col][m_hoveringOverPoint.hexPos.row].getVertex(m_hoveringOverPoint.vertex);
        circle.setCentre(vert);
        g.drawEllipse(circle, 2);
    }
}

void HexGrid::resized()
{
    /* Position hexagons */
    float hexHeight = (getHeight()- (PADDING * 2)) / NUM_ROWS;
    float hexWidth = hexHeight * HEX_W_TO_H_RATIO;

    float curX = PADDING;
    float curY = PADDING;
    float yOffset = hexHeight / 2;
    int numRows = NUM_ROWS;
    for (int i = 0; i < NUM_COLS; i++)
    {
        if (i > 0)
        {
            // Set x to previous column's top right vertex coordinate
            curX = m_hexArray[i-1][0].getVertex(0).x;
        }
        curY = (i % 2 == 0) ? yOffset + PADDING : PADDING;
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

Array<Hexagon*> HexGrid::getNotesToPlay()
{
    Array<Hexagon*> notes;
    for (int i = 0; i < m_tracers.size(); i++)
    {
        Array <Hexagon*> tracerHex = getNotes(m_tracers[i]);
        for (int j = 0; j < tracerHex.size(); j++)
        {
            notes.add(tracerHex[j]);
        }

    }
    return notes;
}

Point<float> HexGrid::getTracerPosition(TracerPoint point)
{
    return m_hexArray[point.hexPos.col][point.hexPos.row].getVertex(point.vertex);
}

void HexGrid::moveTracerRandom(Tracer *tracer)
{
    Array<TracerPoint::Direction> possibleDirs = tracer->position.getMoves();
    int index = Random::getSystemRandom().nextInt(possibleDirs.size());
    return tracer->position.move(possibleDirs[index]);
}

/* returns array of HexTile structs the tracer is currently touching */
Array <Hexagon*> HexGrid::getNotes(Tracer *tracer)
{
    Array <Hexagon*> notes;
    
    /* 8 rows, 15 cols */
    
    switch(tracer->position.intType)
    {
        case TracerPoint::LEFT_T:
        {
            /* internal: one left, two right */
            if (tracer->position.pos.col % 2 == 1)
            {
                if (tracer->position.pos.row == 1)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][0]);
                    notes.add(&m_hexArray[tracer->position.pos.col][0]);
                }
                else if (tracer->position.pos.col == NUM_COLS)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2]);
                }
                else if (tracer->position.pos.row == NUM_ROWS * 2 - 1)
                {
                    /* hex row 7 vertex 5 */
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2]);
                }
                else
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2]);
                }
            }
            else
            {
                notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2]);
                notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
            }

            break;
        }
        case TracerPoint::RIGHT_T:
        {
            if (tracer->position.pos.col % 2 == 1)
            {
                notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
                notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2]);
            }
            else
            {
                if (tracer->position.pos.col == 0)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2]);
                }
                else if (tracer->position.pos.row == 1)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][0]);
                    notes.add(&m_hexArray[tracer->position.pos.col][0]);
                }
                else if (tracer->position.pos.row == NUM_ROWS * 2 - 1)
                {
                    /* hex row 7, odd col */
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
                }
                else
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2]);
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
                }
            }
            break;
        }
        case TracerPoint::LEFT_RIGHT:
        {
            /* only one hex */
            if ( tracer->position.pos.row == 0)
            {
                /* top of odd cols */
                if (tracer->position.vertex == 5)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col][0]);
                }
                else
                {
                    /* vertex = 0 */
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row]);
                }
            }
            else if (tracer->position.pos.row / 2 == NUM_ROWS)
            {
                /* hex in row 7 vertex 2 or 3 */
                if (tracer->position.vertex == 2)
                {
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                }
                else
                {
                    notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
                }
            }
            else if (tracer->position.pos.row == NUM_ROWS * 2 - 1)
            {
                /* bottom of first and last col */
                if (tracer->position.pos.col == 0)
                {
                    /* bottom of col 0 vertex 3 */
                     notes.add(&m_hexArray[0][tracer->position.pos.row / 2 - 1]);
                }
                else if (tracer->position.pos.col == NUM_COLS)
                {
                    /* bottom of last col vertex 2 */
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
                }
            }
            else
            {
                /* top of first and last col */
                if (tracer->position.pos.col == 0)
                {
                    /* top of col 0 vertex 5 */
                     notes.add(&m_hexArray[0][0]);
                }
                else if (tracer->position.pos.col == NUM_COLS)
                {
                    /* top of last col vertex 0 */
                    notes.add(&m_hexArray[tracer->position.pos.col - 1][0]);
                }
            }
            break;
        }
        case TracerPoint::UP_DOWN:
        {
            if (tracer->position.pos.col == 0)
            {
                notes.add(&m_hexArray[tracer->position.pos.col][tracer->position.pos.row / 2 - 1]);
            }
            else{
                /* last col vertex 1*/
                notes.add(&m_hexArray[tracer->position.pos.col - 1][tracer->position.pos.row / 2 - 1]);
            }
            break;
        }
        default:
        {
            jassert(false);
        }
    }
    
    return notes;
}

/* Returns the closest TracerPoint to any point relative to the grid */
TracerPoint HexGrid::getNearestVert(Point<int> pos)
{
    TracerPoint::coord linePosition(-1, -1);
    int vertex = -1;

    /* variance is used to find what column or row the user is in */
    int xVariance = m_hexArray[0][0].getWidth() / 2;
    int yVariance = m_hexArray[0][0].getHeight() / 3;

    /* Find line column */
    if (pos.x <= (xVariance + PADDING)) linePosition.col = 0; // Check first line col
    for (int i = 0; i < NUM_COLS; i++)
    {
        int curX = m_hexArray[i][0].getVertex(0).x;
        if ((pos.x >= (curX - xVariance)) && (pos.x <= (curX + xVariance)))
        {
            linePosition.col = i + 1;
            break;
        }
    }


    for (int i = 0; i < NUM_ROWS; i++)
    {
        int hexCol = (linePosition.col == 0) ? 0 : linePosition.col - 1;
        int yTop = m_hexArray[hexCol][i].getVertex(0).y;
        if ((pos.y <= (yTop + yVariance)))
        {
            if (hexCol % 2) /* If odd col */
            {
                linePosition.row = (2 * i);
            }
            else /* If even col */
            {
                linePosition.row = (2 * i) + 1;
            }
            if (linePosition.col == 0)
            {
                /* Leftmost col */
                if (linePosition.row == 1)
                {
                    /* Top left intersection*/
                    vertex = 5;
                }
                else
                {
                    vertex = 3;
                }
            }
            else
            {
                if (linePosition.row == 0 && (linePosition.col % 2))
                {
                    vertex = 5;
                }
                else
                {
                    vertex = 0;
                }
            }
            break;
        }
        int yMid = m_hexArray[hexCol][i].getVertex(1).y;
        if ((pos.y <= (yMid + yVariance)))
        {
            if (hexCol % 2) /* If odd col */
            {
                linePosition.row = (2 * i) + 1;
            }
            else /* If even col */
            {
                linePosition.row = (2 * i) + 2;
            }
            if (linePosition.col == 0 || ((linePosition.col % 2) && linePosition.row == ((NUM_ROWS * 2) - 1)))
            {
                /* Either first col or bottom hex vert 4 */
                vertex = 4;
            }
            else
            {
                if (hexCol % 2 == linePosition.col % 2)
                {
                    /* Odd hex and line col or even hex and line col */
                    vertex = 0;
                }
                else
                {
                    vertex = 1;
                }
            }
            break;
        }
    }

    /* If row and vertex still not set, must be bottom edge case */
    if (vertex == -1)
    {
        if (linePosition.col == 0)
        {
            /* Bottom left vert 3 */
            linePosition.row = (NUM_ROWS * 2) - 1;
            vertex = 3;
        }
        else if (linePosition.col == NUM_COLS)
        {
            /* Bottom right vert 2 */
            linePosition.row = (NUM_ROWS * 2) - 1;
            vertex = 2;
        }
        else if (linePosition.col % 2)
        {
            linePosition.row = NUM_ROWS * 2;
            vertex = 3;
        }
        else
        {
            linePosition.row = NUM_ROWS * 2;
            vertex = 2;
        }
    }

    DBG("row: " << linePosition.row << ", col: " << linePosition.col << ", vert: " << vertex);
    
    return TracerPoint(linePosition.row, linePosition.col, vertex, false);
}