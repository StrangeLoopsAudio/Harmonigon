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

void Hexagon::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */



    g.fillAll (Colours::transparentBlack);   // clear the background

    Path hexPath, innerPath;
    hexPath.startNewSubPath(getLocalPoint(getParentComponent(), getVertex(0)));
    for (int i = 1; i < 6; i++)
    {
        hexPath.lineTo(getLocalPoint(getParentComponent(), getVertex(i)));
    }
    hexPath.closeSubPath();

    innerPath = hexPath;
    innerPath.applyTransform(AffineTransform::scale(0.92, 0.92, getWidth() / 2, getHeight() / 2));

    g.setColour(Colours::purple);
    g.strokePath(hexPath, PathStrokeType(1.0f));
    g.setColour(m_curColour);
    g.strokePath(innerPath, PathStrokeType(1.0f));

    g.setColour (Colours::white);
    g.setFont (20.0f);
    g.drawText (NoteUtils::keyToString(m_tile.key), getLocalBounds(),
            Justification::centred, true);   // draw some placeholder text

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
