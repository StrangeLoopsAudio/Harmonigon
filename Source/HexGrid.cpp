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
    m_curPathColour = getNextColour();
    m_isHexMode = isHexPath;
    repaint();
}

void HexGrid::endPath()
{
    m_canSelect = false;
    m_selectedHexes.clear();
    m_tracerStart.intType = TracerPoint::INVALID;
    m_hoveringOverPoint.intType = TracerPoint::INVALID;
    m_hoveringOverHex = nullptr;
    m_pathDirs.clear();
    repaint();
}

void HexGrid::storePath(HarmonigonPath* path)
{
    m_paths.add(path);
    m_numPaths++;
    if (!path->isHexPath)
    {
        /* Create tracer for path */
        Tracer* newTracer = new Tracer(path->tracerStart, path);
        newTracer->setSize(15, 15);
        m_tracers.add(newTracer);
        addAndMakeVisible(newTracer);
        resized();
    }
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
            Tracer* newTracer = new Tracer(path->tracerStart, path);
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
            if (m_tracerStart.intType != TracerPoint::INVALID)
            {
                m_hoveringOverPoint.intType = TracerPoint::INVALID;
                TracerPoint endPath = m_tracerStart.getPathEnd(m_pathDirs);
                Array<TracerPoint::Direction> moves = m_tracerStart.getValidNextMoves(m_pathDirs);
                for (TracerPoint::Direction dir : moves)
                {
                    TracerPoint nextVertOption = endPath;
                    nextVertOption.move(dir);
                    if (nearestPoint == nextVertOption)
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
}

void HexGrid::mouseExit(const MouseEvent& event)
{
    if (m_canSelect)
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
            if (m_tracerStart.intType == TracerPoint::INVALID)
            {
                m_tracerStart = m_hoveringOverPoint;
            }
            else
            {
                TracerPoint pathEnd = m_tracerStart.getPathEnd(m_pathDirs);
                Array<TracerPoint::Direction> moves = m_tracerStart.getValidNextMoves(m_pathDirs);
                for (TracerPoint::Direction dir : moves)
                {
                    TracerPoint nextVertOption = pathEnd;
                    nextVertOption.move(dir);
                    if (m_hoveringOverPoint == nextVertOption)
                    {
                        m_pathDirs.add(dir);
                    }
                }
            }
        }
        repaint();
    }
}

void HexGrid::advancePaths(int quarterNoteDuration)
{
    for (int i = 0; i < m_tracers.size(); i++)
    {
        Rectangle<int> center = m_tracers[i]->getBounds();
        m_tracers[i]->advancePath();
        center.setCentre(getTracerPosition(m_tracers[i]->getPoint()).toInt());
        m_animator.animateComponent(m_tracers[i], center, 1, quarterNoteDuration - 10, true, 0.3, 0.3);
        repaint();
        resized();
    }
    for (HarmonigonPath* path : m_paths)
    {
        if (path->isHexPath)
        {
            path->curIndex++;
            if (path->curIndex >= path->selectedHexes.size())
            {
                path->curIndex = 0;
            }
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
        Rectangle<float> circle(0, 0, 10, 10);
        Point<float> vert = m_hexArray[m_hoveringOverPoint.hexPos.col][m_hoveringOverPoint.hexPos.row].getVertex(m_hoveringOverPoint.vertex);
        circle.setCentre(vert);
        g.drawEllipse(circle, 2);
    }
    if (m_tracerStart.intType != TracerPoint::INVALID)
    {
        g.setColour(m_curPathColour);
        Point<float> vert = m_hexArray[m_tracerStart.hexPos.col][m_tracerStart.hexPos.row].getVertex(m_tracerStart.vertex);
        TracerPoint curPoint = m_tracerStart;
        Path tracerPath;
        tracerPath.startNewSubPath(vert);
        for (TracerPoint::Direction dir : m_pathDirs)
        {
            curPoint.move(dir);
            tracerPath.lineTo(m_hexArray[curPoint.hexPos.col][curPoint.hexPos.row].getVertex(curPoint.vertex));
        }
        g.strokePath(tracerPath, PathStrokeType(4.0f));
        
        if (m_canSelect && !m_isHexMode)
        {
            /* Draw possible next moves */
            g.setColour(Colours::aqua);
            TracerPoint end = m_tracerStart.getPathEnd(m_pathDirs);
            Array<TracerPoint::Direction> possibleMoves = m_tracerStart.getValidNextMoves(m_pathDirs);
            for (TracerPoint::Direction dir : possibleMoves)
            {
                TracerPoint point = end;
                point.move(dir);
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
            Point<float> vert = m_hexArray[path->tracerStart.hexPos.col][path->tracerStart.hexPos.row].getVertex(path->tracerStart.vertex);
            TracerPoint curPoint = path->tracerStart;
            Path tracerPath;
            tracerPath.startNewSubPath(vert);
            for (TracerPoint::Direction dir : path->pathDirs)
            {
                curPoint.move(dir);
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

HarmonigonPath* HexGrid::getPath()
{
    HarmonigonPath* newPath;
    if (m_isHexMode)
    {
        newPath = new HarmonigonPath(m_numPaths, m_curPathColour, m_selectedHexes);
    }
    else
    {
        newPath = new HarmonigonPath(m_numPaths, m_curPathColour, m_tracerStart, m_pathDirs);
    }
    return newPath;
}

Colour HexGrid::getNextColour()
{
    return m_colours[m_numPaths % m_colours.size()];
}

Array<Hexagon*> HexGrid::getNotesToPlay()
{
    Array<Hexagon*> notes;
    for (HarmonigonPath *path : m_paths)
    {
        if (path->isHexPath)
        {
            notes.add(path->selectedHexes[path->curIndex]);
        }
    }
    for (Tracer* tracer : m_tracers)
    {
        notes.addArray(getNotes(tracer));
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
Array <Hexagon*> HexGrid::getNotes(Tracer *tracer)
{
    Array <Hexagon*> notes;
    
    /* 8 rows, 15 cols */
    TracerPoint point = tracer->getPoint();
    
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

    //DBG("row: " << linePosition.row << ", col: " << linePosition.col << ", vert: " << vertex);
    
    return TracerPoint(linePosition.row, linePosition.col, vertex, false);
}