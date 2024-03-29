/*
  ==============================================================================

    Tracer.h
    Created: 14 Jun 2020 3:32:20pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Hexagon.h"
#include "TracerPoint.h"
#include "HarmonigonPath.h"

using namespace juce;

//==============================================================================

class Tracer : public Component
{
public:
    Tracer(TracerPoint position, HarmonigonPath* path);
    ~Tracer();

    void paint (Graphics&) override;
    void resized() override;

    TracerPoint getPoint();
    void move(TracerPoint::Direction dir);
    HarmonigonPath* getPath();
    TracerPoint getPosition();
    void setPosition(TracerPoint newPosition);
//    void advancePath();

private:

    TracerPoint m_position;
    HarmonigonPath* m_path;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tracer)
};
