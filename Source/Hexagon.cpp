/*
  ==============================================================================

    Hexagon.cpp
    Created: 14 Jun 2020 3:32:11pm
    Author:  brady

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Hexagon.h"

//==============================================================================
Hexagon::Hexagon()
{
    m_noteNum = 0;
    m_sideLength = 0;
    m_curColour = Colours::purple;
}

Hexagon::~Hexagon()
{
    m_center = Point<float>(getX() + (getWidth() / 2), getY() + (getHeight() / 2));
    m_sideLength = getWidth() / 2;
}

bool Hexagon::hitTest(int x, int y)
{
    Point<float> point(x + .5f, y + .5f);
    return m_hexPath.contains(point, 5.f);
}

void Hexagon::paint (Graphics& g)
{
    g.fillAll (Colours::transparentBlack);   // clear the background

    m_hexPath.clear();
    m_hexPath.startNewSubPath(getLocalPoint(getParentComponent(), getVertex(0)));
    for (int i = 1; i < 6; i++)
    {
        m_hexPath.lineTo(getLocalPoint(getParentComponent(), getVertex(i)));
    }
    m_hexPath.closeSubPath();
    g.setColour(Colours::black);
    g.strokePath(m_hexPath, PathStrokeType(1.0f));

    Path selectedPath, pulsePath;

    /* Draw selected path */
    selectedPath = m_hexPath;
    selectedPath.applyTransform(AffineTransform::scale(0.89, 0.89, getWidth() / 2, getHeight() / 2));

    if (m_isSelected)
    {
        g.setColour(m_pathColour);
    }
    else if (m_isHovering)
    {
        g.setColour(Colours::aqua);
    }
    else
    {
        g.setColour(Colours::purple);
    }
    if (m_isFirst)
    {
        g.strokePath(selectedPath, PathStrokeType(4.0f));
    }
    else
    {
        g.strokePath(selectedPath, PathStrokeType(2.0f));
    }

    /* Draw pulse path */
    pulsePath = m_hexPath;
    pulsePath.applyTransform(AffineTransform::scale(0.91, 0.91, getWidth() / 2, getHeight() / 2));

    g.setColour(m_curColour);
    g.strokePath(pulsePath, PathStrokeType(1.0f));

    /* Draw note text */
    g.setColour (Colours::white);
    g.setFont (20.0f);
    g.drawText (NoteUtils::keyToString(m_tile.key), getLocalBounds(), Justification::centred, true);
}

/* Returns vertex coordinates relative to parent */
Point<float> Hexagon::getVertex(int index) {
    jassert(index >= 0 && index < 6);
    int a = (getHeight() / 2);
    
    /* Top right vertex is index 0, clockwise increasing */
    switch (index) {
    case 0:
        return Point<float>(m_center.x + (m_sideLength / 2), m_center.y - a);
        break;
    case 1:
        return Point<float>(m_center.x + m_sideLength, m_center.y);
        break;
    case 2:
        return Point<float>(m_center.x + (m_sideLength / 2), m_center.y + a);
        break;
    case 3:
        return Point<float>(m_center.x - (m_sideLength / 2), m_center.y + a);
        break;
    case 4:
        return Point<float>(m_center.x - m_sideLength, m_center.y);
        break;
    case 5:
        return Point<float>(m_center.x - (m_sideLength / 2), m_center.y - a);
        break;
    }
}

void Hexagon::timerCallback()
{
    if (m_timerCount > 10)
    {
        stopTimer();
        m_curColour = Colours::purple;
        repaint();
        return;
    }
    m_curColour = m_curColour.brighter();
    m_timerCount++;
    repaint();
}

void Hexagon::pulse()
{
    if (!isTimerRunning())
    {
        m_timerCount = 0;
        startTimer(20);
    }
}

void Hexagon::setNote(int noteNum)
{
    m_noteNum = noteNum;
}

void Hexagon::setPosition(int row, int col)
{
    m_row = row;
    m_col = col;
}

int Hexagon::getRow()
{
    return m_row;
}

int Hexagon::getCol()
{
    return m_col;
}

void Hexagon::setHovering(bool isHovering)
{
    m_isHovering = isHovering;
    repaint();
}

void Hexagon::setSelected(Colour colour, bool isFirst)
{
    m_isSelected = true;
    m_isFirst = isFirst;
    m_pathColour = colour;
    repaint();
}

void Hexagon::endSelected()
{
    m_isSelected = false;
    repaint();
}

bool Hexagon::isSelected()
{
    return m_isSelected;
}

void Hexagon::setTile(NoteUtils::HexTile tile)
{
    m_tile = tile;
}

NoteUtils::HexTile Hexagon::getTile()
{
    return m_tile;
}

void Hexagon::resized()
{
    m_center = Point<float>(getX() + (getWidth() / 2), getY() + (getHeight() / 2));
    m_sideLength = getWidth() / 2;
}
