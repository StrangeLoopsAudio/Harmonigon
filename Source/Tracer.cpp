/*
  ==============================================================================

    Tracer.cpp
    Created: 14 Jun 2020 3:32:20pm
    Author:  brady

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Tracer.h"

//==============================================================================

Tracer::Tracer(TracerPoint position, HarmonigonPath* path) : m_position(position), m_path(path)
{
}

Tracer::~Tracer()
{
}

TracerPoint Tracer::getPoint()
{
    return m_position;
}

void Tracer::move(TracerPoint::Direction dir)
{
    m_position.move(dir);
}

void Tracer::paint (Graphics& g)
{
    g.setColour(m_path->colour);
    Rectangle<float> circle = getLocalBounds().toFloat().reduced(getWidth() / 3);
    circle.setCentre(getWidth() / 2, getHeight() / 2);
    g.drawEllipse(circle, 2);
}

void Tracer::resized()
{
}

