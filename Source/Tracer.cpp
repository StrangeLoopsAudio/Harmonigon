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

void TracerPoint::initLinePos()
{
    if (vertex < 2)
    {
        /* General case for vertices 0, 1 */
        pos.col = hexPos.col + 1;
        if (hexPos.col % 2 == 0)
        {
            pos.row = (2 * hexPos.row) + vertex + 1;
        }
        else
        {
            pos.row = (2 * hexPos.row) + vertex;
        }
    }
    else {
        /* Edge cases */
        if (vertex > 2)
        {
            pos.col = hexPos.col;
        }
        else
        {
            pos.col = hexPos.col + 1;
        }

        if (hexPos.row == 0)
        {
            /* Odd column top row */
            pos.row = 0;
        }
        else if (hexPos.col % 2)
        {
            /* Odd column bottom row */
            if (vertex > 3)
            {
                pos.row = (NUM_ROWS * 2) - 1;
            }
            else
            {
                pos.row = (NUM_ROWS * 2);
            }
        }
        else if (hexPos.col == 0)
        {
            if (hexPos.row == 0 && vertex == 5)
            {
                /* Top left tile */
                pos.row = 1;
            }
            else
            {
                /* First column */
                pos.row = (2 * hexPos.row) + (5 - vertex) + 1;
            }
        }
        else if (hexPos.col == NUM_COLS - 1)
        {
            /* Bottom right tile */
            pos.row = (NUM_ROWS * 2) - 1;
        }
        else
        {
            // Logic must be wrong, edge case not caught */
            jassert(false);
        }
    }
}

void TracerPoint::refreshHexPos()
{
    /* Assuming new line pos has been set, need to update hexPos to match */
    /* Refresh column */
    if (vertex > 2)
    {
        /* General case for vertices 0, 1 */
        hexPos.col = pos.col;
        if (hexPos.col % 2 == 0)
        {
            hexPos.row = (pos.row - vertex - 1) / 2;
        }
        else
        {
            hexPos.row = (pos.row - vertex) / 2;
        }
    }
    else
    {
        /* Edge cases */
        if (vertex > 2)
        {
            hexPos.col = pos.col;
        }
        else
        {
            hexPos.col = pos.col - 1;
        }

        if (pos.row == 0)
        {
            /* Odd column top row */
            hexPos.row = 0;
        }
        else if (pos.row == (NUM_ROWS * 2) || (pos.row == ((NUM_ROWS * 2) - 1) && vertex == 4))
        {
            /* Odd column bottom row */
            hexPos.row = NUM_ROWS - 1;
        }
        else if (pos.col == 0)
        {
            if (pos.row == 1 && vertex == 5)
            {
                /* Top left tile */
                hexPos.row = 0;
            }
            else
            {
                /* First column */
                hexPos.row = (pos.row - 1 - (5 - vertex)) / 2;
            }
        }
        else if (hexPos.col == NUM_COLS - 1)
        {
            /* Bottom right tile */
            hexPos.row = NUM_ROWS - 1;
        }
    }
}

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
    g.setColour (Colours::deeppink);
    g.drawEllipse(getLocalBounds().toFloat(), 2);
}

void Tracer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

