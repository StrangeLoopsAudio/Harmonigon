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
Tracer::Tracer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Tracer::~Tracer()
{
}

void Tracer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::deeppink);
    g.drawEllipse(getLocalBounds().toFloat(), 2);
}

void Tracer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

