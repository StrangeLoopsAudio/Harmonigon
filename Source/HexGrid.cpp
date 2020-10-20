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

    m_colours =
    {
        Colours::crimson,
        Colours::goldenrod,
        Colours::palegreen,
        Colours::orchid,
        Colours::peachpuff,
        Colours::lavender
    };

    setInterceptsMouseClicks(false, true);
}

void HexGrid::startNewPath(bool isHexPath)
{
    m_canSelect = true;
    m_pathStarted = true;
    m_curPathColour = getNextColour();
    m_activeColours.add(m_curPathColour);
    m_isHexMode = isHexPath;
    repaint();
}

void HexGrid::endPath()
{
    m_canSelect = false;
    m_pathStarted = false;
    m_selectedHexes.clear();
    m_hoveringOverPoint.intType = TracerPoint::INVALID;
    m_hoveringOverHex = nullptr;

    m_tracerLinePath.clear();
    repaint();
}

void HexGrid::storePath(HarmonigonPath* path)
{
    Array<TracerPoint*> tracerLinePathCopy;
    for (int i = 0; i < path->tracerLinePath.size(); i++)
    {
        TracerPoint* pointCopy = new TracerPoint(path->tracerLinePath[i]->pos.row, path->tracerLinePath[i]->pos.col, path->tracerLinePath[i]->vertex, false);
        tracerLinePathCopy.add(pointCopy);
    }
    path->tracerLinePath = tracerLinePathCopy;
    m_paths.add(path);
    m_numPaths++;
    if (!path->isHexPath)
    {
        /* Create tracer for path */
        Tracer* newTracer = new Tracer(*path->tracerLinePath[0], path);
        newTracer->setSize(15, 15);
        m_tracers.add(newTracer);
        addAndMakeVisible(newTracer);
        resized();
    }
    repaint();
}

void HexGrid::deletePath(HarmonigonPath* path)
{
    if(path->isHexPath)
    {
        for(Hexagon* hex : path->hexPath)
        {
            hex->clearColor();
            hex->repaint();
        }
    }
    m_activeColours.removeAllInstancesOf(path->colour);
    m_paths.removeObject(path);
//    m_numPaths--;
    resetPathPositions();
    repaint();

}

void HexGrid::resetPathPositions()
{
    m_tracers.clear();
    for (HarmonigonPath* path : m_paths)
    {
        path->curIndex = 0;
        if (!path->isHexPath)
        {
            Tracer* newTracer = new Tracer(*path->tracerLinePath[0], path);
            newTracer->setSize(15, 15);
            m_tracers.add(newTracer);
            addAndMakeVisible(newTracer);
        }
    }
    resized();
}

void HexGrid::mouseMove(const MouseEvent& e)
{
    if (m_canSelect)
    {
        if (m_isHexMode)
        {
            Hexagon* hex = (Hexagon*)e.eventComponent;
            if (m_selectedHexes.size() > 0)
            {
                m_hoveringOverHex = nullptr;
                Array<Hexagon *> moves = getAdjacentHexes();
                for (Hexagon* move : moves)
                {
                    if (move == hex)
                    {
                        if (m_hoveringOverHex != nullptr)
                        {
                            m_hoveringOverHex->setHovering(false);
                        }
                        m_hoveringOverHex = hex;
                        hex->setHovering(true);
                    }
                }
            }
            else
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
            TracerPoint nearestPoint = getNearestVert(getLocalPoint(e.eventComponent, e.getMouseDownPosition()));
            if (m_tracerLinePath.size() > 0)
            {
                m_hoveringOverPoint.intType = TracerPoint::INVALID;
                Array<TracerPoint> moves = HarmonigonPath::getValidNextMoves(m_tracerLinePath);
                for (TracerPoint point : moves)
                {
                    if (nearestPoint == point)
                    {
                        m_hoveringOverPoint = nearestPoint;
                    }
                }
            }
            else
            {
                m_hoveringOverPoint = nearestPoint;
            }

        }

        repaint();
    }
    else
    {
        if (m_isHexMode)
        {
            Hexagon* hex = (Hexagon*)e.eventComponent;
            if (m_hoveringOverHex != nullptr)
            {
                m_hoveringOverHex->setHovering(false);
            }
            m_hoveringOverHex = hex;
            hex->setHovering(true);
        }
        else
        {
            TracerPoint nearestPoint = getNearestVert(getLocalPoint(e.eventComponent, e.getMouseDownPosition()));
            m_hoveringOverPoint = nearestPoint;
            repaint();
        }
    }
}

void HexGrid::mouseExit(const MouseEvent& event)
{
    if (m_isHexMode)
    {
        if (m_hoveringOverHex != nullptr)
        {
            m_hoveringOverHex->setHovering(false);
            m_hoveringOverHex = nullptr;
        }
    }
    else
    {
        /* Move offscreen if mouse is off of grid */
        m_hoveringOverPoint.intType = TracerPoint::INVALID;
        repaint();
    }
}

void HexGrid::mouseDown(const MouseEvent& event)
{
    if (m_canSelect)
    {
        if (m_isHexMode && m_hoveringOverHex != nullptr)
        {
            /* Only add if isn't already selected */
            if (!m_hoveringOverHex->isSelected())
            {
                m_hoveringOverHex->setSelected(m_curPathColour, m_selectedHexes.isEmpty());
                m_selectedHexes.add(m_hoveringOverHex);
            }
        }
        else if (!m_isHexMode && m_hoveringOverPoint.intType != TracerPoint::INVALID)
        {
            if (m_tracerLinePath.size() == 0)
            {

                TracerPoint* hoveringCopy = new TracerPoint(m_hoveringOverPoint.pos.row, m_hoveringOverPoint.pos.col, m_hoveringOverPoint.vertex, false);
                m_tracerLinePath.add(hoveringCopy);
            }
            else
            {
                Array<TracerPoint> moves = HarmonigonPath::getValidNextMoves(m_tracerLinePath);
                for (TracerPoint point : moves)
                {
                    if (m_hoveringOverPoint == point)
                    {
                        TracerPoint* newPoint = new TracerPoint(point.pos.row, point.pos.col, point.vertex, false);
                        m_tracerLinePath.add(newPoint);
                    }
                }
            }
        }
        repaint();
    }
    else
    {
        /* If hovering over chord or hex, trigger callback in parent */
        if (m_hoveringOverHex != nullptr || m_hoveringOverPoint.intType != TracerPoint::INVALID)
        {
            onButtonPressed();
        }
    }
}

void HexGrid::mouseUp(const MouseEvent& event)
{
    if (!m_canSelect)
    {
        if (m_hoveringOverHex != nullptr || m_hoveringOverPoint.intType != TracerPoint::INVALID)
        {
            /* Stop current note of chord */
            onButtonReleased();
        }
    }
}

void HexGrid::setSelectionType(bool isHex)
{
    m_isHexMode = isHex;
}

void HexGrid::advancePaths(int sixteenthNoteDuration)
{
    for (int i = 0; i < m_tracers.size(); i++)
    {
        if(m_tracers[i]->getPath()->notesPlaying)
        {
            Rectangle<int> center = m_tracers[i]->getBounds();
            if (m_tracers[i]->getPath()->curIndex >= m_tracers[i]->getPath()->tracerLinePath.size() - 1)
            {
                m_tracers[i]->setPosition(*(m_tracers[i]->getPath()->tracerLinePath[0]));
                m_tracers[i]->getPath()->curIndex = 0;
            }
            else
            {
                m_tracers[i]->setPosition(*(m_tracers[i]->getPath()->tracerLinePath[m_tracers[i]->getPath()->curIndex + 1]));
                m_tracers[i]->getPath()->curIndex++;
            }
            center.setCentre(getTracerPosition(m_tracers[i]->getPoint()).toInt());
            m_animator.animateComponent(m_tracers[i], center, 1, sixteenthNoteDuration - 10, true, 0.3, 0.3);
            repaint();
            resized();
        }
    }
    for (HarmonigonPath* path : m_paths)
    {
        if (path->isHexPath && path->notesPlaying)
        {
            path->curIndex++;
            if (path->curIndex >= path->hexPath.size())
            {
                path->curIndex = 0;
            }
        }
        path->notesPlaying = false;
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
        Rectangle<float> circle(0, 0, 10, 10);
        Point<float> vert = m_hexArray[m_hoveringOverPoint.hexPos.col][m_hoveringOverPoint.hexPos.row].getVertex(m_hoveringOverPoint.vertex);
        circle.setCentre(vert);
        g.drawEllipse(circle, 2);
    }
    if (m_tracerLinePath.size() > 0)
    {
        g.setColour(m_curPathColour);
        Point<float> vert = m_hexArray[m_tracerLinePath[0]->hexPos.col][m_tracerLinePath[0]->hexPos.row].getVertex(m_tracerLinePath[0]->vertex);
        TracerPoint curPoint = *m_tracerLinePath[0];
        Path tracerPath;
        tracerPath.startNewSubPath(vert);
        for (TracerPoint* point : m_tracerLinePath)
        {
            curPoint = *point;
            tracerPath.lineTo(m_hexArray[curPoint.hexPos.col][curPoint.hexPos.row].getVertex(curPoint.vertex));
        }
        g.strokePath(tracerPath, PathStrokeType(4.0f));

        if (m_canSelect && !m_isHexMode)
        {
            /* Draw possible next moves */
            g.setColour(Colours::aqua);
            TracerPoint end = *m_tracerLinePath.getLast();
            Array<TracerPoint> possibleMoves = HarmonigonPath::getValidNextMoves(m_tracerLinePath);
            for (TracerPoint point : possibleMoves)
            {
                g.drawLine(Line<float>(m_hexArray[end.hexPos.col][end.hexPos.row].getVertex(end.vertex),
                                       m_hexArray[point.hexPos.col][point.hexPos.row].getVertex(point.vertex)), 4.f);
            }
        }
    }

    /* Draw completed paths */
    for (HarmonigonPath* path : m_paths)
    {
        if (!path->isHexPath)
        {
            g.setColour(path->colour);
            Point<float> vert = m_hexArray[path->tracerLinePath[0]->hexPos.col][path->tracerLinePath[0]->hexPos.row].getVertex(path->tracerLinePath[0]->vertex);
            TracerPoint curPoint = *path->tracerLinePath[0];
            Path tracerPath;
            tracerPath.startNewSubPath(vert);
            for (int i = 0; i < path->tracerLinePath.size(); i++)
            {
                curPoint = *path->tracerLinePath[i];
                tracerPath.lineTo(m_hexArray[curPoint.hexPos.col][curPoint.hexPos.row].getVertex(curPoint.vertex));
            }
            g.strokePath(tracerPath, PathStrokeType(4.0f));
        }
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
        Point<float> tracerPos = getTracerPosition(m_tracers[i]->getPoint());
        m_tracers[i]->setCentrePosition(tracerPos.toInt());
    }
}

HarmonigonPath* HexGrid::createPath()
{
    HarmonigonPath* newPath;
    if (m_isHexMode)
    {
        newPath = new HarmonigonPath(m_numPaths, m_curPathColour, m_selectedHexes);
    }
    else
    {
        newPath = new HarmonigonPath(m_numPaths, m_curPathColour, m_tracerLinePath);
    }
    return newPath;
}

Colour HexGrid::getNextColour()
{
    for (Colour colour: m_colours)
    {
        if (!m_activeColours.contains(colour))
        {
            return colour;
        }
    }
    return Colours::purple;
}

Array<Hexagon*> HexGrid::getNotesToPlay()
{
    Array<Hexagon*> notes;
    for (HarmonigonPath *path : m_paths)
    {
        if(noteIntervalCount % path->noteIncrement == 0)
        {
            path->notesPlaying = true;
            if (path->isHexPath)
            {
                notes.add(path->hexPath[path->curIndex]);
            }
            else
            {
                notes.addArray(getNotes(*path->tracerLinePath[path->curIndex]));
            }
        }

    }

    noteIntervalCount++;
    if(noteIntervalCount > 16) noteIntervalCount = 1;
    return notes;
}

Array<Hexagon*> HexGrid::getNotesToTurnOff()
{
    Array<Hexagon*> notes;
    for (HarmonigonPath *path : m_paths)
    {
        if(noteIntervalCount % path->noteIncrement == 0)
        {
            if (path->isHexPath)
            {
                if(path->curIndex == 0)
                {
                    notes.add(path->hexPath[path->getHexPath().size() - 1]);

                }
                else
                {
                    notes.add(path->hexPath[path->curIndex - 1]);

                }
            }
            else
            {
                if(path->curIndex == 0)
                {
                    notes.addArray(getNotes(*path->tracerLinePath[path->tracerLinePath.size() - 1]));

                }
                else
                {
                    notes.addArray(getNotes(*path->tracerLinePath[path->curIndex - 1]));

                }
            }
        }

    }

    return notes;
}


Array<Hexagon*> HexGrid::getFreePlayNotes()
{
    Array<Hexagon*> notes;
    if (m_isHexMode && m_hoveringOverHex != nullptr)
    {
        notes.add(m_hoveringOverHex);
    }
    else if (!m_isHexMode && m_hoveringOverPoint.intType != TracerPoint::INVALID)
    {
        notes.addArray(getNotes(m_hoveringOverPoint));
    }
    return notes;
}

Point<float> HexGrid::getTracerPosition(TracerPoint point)
{
    return m_hexArray[point.hexPos.col][point.hexPos.row].getVertex(point.vertex);
}

void HexGrid::moveTracerRandom(Tracer *tracer)
{
    Array<TracerPoint::Direction> possibleDirs = tracer->getPoint().getMoves();
    int index = Random::getSystemRandom().nextInt(possibleDirs.size());
    return tracer->move(possibleDirs[index]);
}

/* Returns array of HexTile structs the tracer is currently touching */
Array <Hexagon*> HexGrid::getNotes(TracerPoint point)
{
    Array <Hexagon*> notes;

    /* 8 rows, 15 cols */
    switch(point.intType)
    {
        case TracerPoint::LEFT_T:
        {
            /* internal: one left, two right */
            if (point.pos.col % 2 == 1)
            {
                if (point.pos.row == 1)
                {
                    notes.add(&m_hexArray[point.pos.col - 1][0]);
                    notes.add(&m_hexArray[point.pos.col][0]);
                }
                else if (point.pos.col == NUM_COLS)
                {
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2]);
                }
                else if (point.pos.row == NUM_ROWS * 2 - 1)
                {
                    /* hex row 7 vertex 5 */
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2]);
                }
                else
                {
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2]);
                }
            }
            else
            {
                notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2]);
                notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
            }

            break;
        }
        case TracerPoint::RIGHT_T:
        {
            if (point.pos.col % 2 == 1)
            {
                notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
                notes.add(&m_hexArray[point.pos.col][point.pos.row / 2]);
            }
            else
            {
                if (point.pos.col == 0)
                {
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2]);
                }
                else if (point.pos.row == 1)
                {
                    notes.add(&m_hexArray[point.pos.col - 1][0]);
                    notes.add(&m_hexArray[point.pos.col][0]);
                }
                else if (point.pos.row == NUM_ROWS * 2 - 1)
                {
                    /* hex row 7, odd col */
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
                }
                else
                {
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2]);
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
                }
            }
            break;
        }
        case TracerPoint::LEFT_RIGHT:
        {
            /* only one hex */
            if ( point.pos.row == 0)
            {
                /* top of odd cols */
                if (point.vertex == 5)
                {
                    notes.add(&m_hexArray[point.pos.col][0]);
                }
                else
                {
                    /* vertex = 0 */
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row]);
                }
            }
            else if (point.pos.row / 2 == NUM_ROWS)
            {
                /* hex in row 7 vertex 2 or 3 */
                if (point.vertex == 2)
                {
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                }
                else
                {
                    notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
                }
            }
            else if (point.pos.row == NUM_ROWS * 2 - 1)
            {
                /* bottom of first and last col */
                if (point.pos.col == 0)
                {
                    /* bottom of col 0 vertex 3 */
                     notes.add(&m_hexArray[0][point.pos.row / 2 - 1]);
                }
                else if (point.pos.col == NUM_COLS)
                {
                    /* bottom of last col vertex 2 */
                    notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
                }
            }
            else
            {
                /* top of first and last col */
                if (point.pos.col == 0)
                {
                    /* top of col 0 vertex 5 */
                     notes.add(&m_hexArray[0][0]);
                }
                else if (point.pos.col == NUM_COLS)
                {
                    /* top of last col vertex 0 */
                    notes.add(&m_hexArray[point.pos.col - 1][0]);
                }
            }
            break;
        }
        case TracerPoint::UP_DOWN:
        {
            if (point.pos.col == 0)
            {
                notes.add(&m_hexArray[point.pos.col][point.pos.row / 2 - 1]);
            }
            else{
                /* last col vertex 1*/
                notes.add(&m_hexArray[point.pos.col - 1][point.pos.row / 2 - 1]);
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

Array<Hexagon*> HexGrid::getAdjacentHexes()
{
    Array<Hexagon*> hexes;
    Hexagon* end = m_selectedHexes.getLast();
    bool isOdd = end->getCol() % 2;
    if (end->getRow() != 0)
    {
        /* Add top hex */
        hexes.add(&m_hexArray[end->getCol()][end->getRow() - 1]);
    }
    if ((end->getRow() != NUM_ROWS - 1 && isOdd) || (end->getRow() != NUM_ROWS - 2 && !isOdd))
    {
        /* Add bottom hex */
        hexes.add(&m_hexArray[end->getCol()][end->getRow() + 1]);
    }
    if (end->getCol() != 0)
    {
        if ((end->getRow() != 0 && isOdd) || !isOdd)
        {
            /* Add top left hex */
            if (isOdd)
            {
                hexes.add(&m_hexArray[end->getCol() - 1][end->getRow() - 1]);
            }
            else
            {
                hexes.add(&m_hexArray[end->getCol() - 1][end->getRow()]);
            }
        }
        if ((end->getRow() != NUM_ROWS - 1 && isOdd) || !isOdd)
        {
            /* Add bottom left hex */
            if (isOdd)
            {
                hexes.add(&m_hexArray[end->getCol() - 1][end->getRow()]);
            }
            else
            {
                hexes.add(&m_hexArray[end->getCol() - 1][end->getRow() + 1]);
            }
        }
    }
    if (end->getCol() != NUM_COLS - 1)
    {
        if ((end->getRow() != 0 && isOdd) || !isOdd)
        {
            /* Add top right hex */
            if (isOdd)
            {
                hexes.add(&m_hexArray[end->getCol() + 1][end->getRow() - 1]);
            }
            else
            {
                hexes.add(&m_hexArray[end->getCol() + 1][end->getRow()]);
            }
        }
        if ((end->getRow() != NUM_ROWS - 1 && isOdd) || !isOdd)
        {
            /* Add bottom right hex */
            if (isOdd)
            {
                hexes.add(&m_hexArray[end->getCol() + 1][end->getRow()]);
            }
            else
            {
                hexes.add(&m_hexArray[end->getCol() + 1][end->getRow() + 1]);
            }
        }
    }
    return hexes;
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

    if (linePosition.col == -1)
    {
        /* Rightmost line col */
        linePosition.col = NUM_COLS;
    }
    int hexCol = (linePosition.col == 0) ? 0 : linePosition.col - 1;
    if ((pos.y <= (yVariance + PADDING)) && (linePosition.col % 2) && linePosition.col < NUM_COLS)
    {
        hexCol = linePosition.col;
    }
    else if ((pos.y >= m_hexArray[1][7].getVertex(0).y + yVariance) &&
            (pos.y <= m_hexArray[1][7].getVertex(2).y - yVariance) &&
            linePosition.col % 2 &&
            linePosition.col < NUM_COLS)
    {
        /* Holy fucking edge case, bottom odd col vert 4 */
        hexCol = linePosition.col;
    }
    for (int i = 0; i < NUM_ROWS; i++)
    {
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

    return TracerPoint(linePosition.row, linePosition.col, vertex, false);
}
