/*
  ==============================================================================

    NoteUtils.h
    Created: 14 Jun 2020 9:00:21pm
    Author:  brady

  ==============================================================================
*/

#pragma once
#include <tuple>
using namespace std;

#define NUM_ROWS 8
#define NUM_COLS 15

enum Key
{
    C = 0,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B
};

String keyToString(Key key)
{
    switch (key)
    {
    case C:
        return "C";
        break;
    case Cs:
        return "C#";
        break;
    case D:
        return "D";
        break;
    case Ds:
        return "D#";
        break;
    case E:
        return "E";
        break;
    case F:
        return "F";
        break;
    case Fs:
        return "F#";
        break;
    case G:
        return "G";
        break;
    case Gs:
        return "G#";
        break;
    case A:
        return "A";
        break;
    case As:
        return "A#";
        break;
    case B:
        return "B";
        break;
    }
}

float hexToFreq(hexagon hex)
{
    return freqs[hex.octave - 2][hex.key];
}

float freqs[5][12] = {
    /*          C       C#       D        D#       E        F       F#        G       G#        A        A#       B   */
    /*2 */  {65.41f,  69.3f,   73.42f,  77.78f,  82.41f,  87.31f,  92.5f,   98.0f,   103.8f,  110.0f,  116.5f,  123.5f},
    /*3 */  {130.8f,  138.6f,  146.8f,  155.6f,  164.8f,  174.6f,  185.0f,  196.0f,  207.7f,  220.0f,  233.1f,  246.9f},
    /*4 */  {261.6f,  277.2f,  293.7f,  311.1f,  329.6f,  349.2f,  370.0f,  392.0f,  415.3f,  440.0f,  466.2f,  493.9f},
    /*5 */  {523.3f,  554.4f,  587.3f,  622.3f,  659.3f,  698.5f,  740.0f,  784.0f,  830.6f,  880.0f,  932.3f,  987.8f},
    /*6 */  {1047.0f, 1109.0f, 1175.0f, 1245.0f, 1319.0f, 1397.0f, 1480.0f, 1568.0f, 1661.0f, 1760.0f, 1865.0f, 1976.0f}
};

typedef struct _hexagon
{
    int octave;
    Key key;
} hexagon;

hexagon hexagons[NUM_ROWS][NUM_COLS] = {
    { {6, Cs}, {6, F }, {6, D }, {6, Fs}, {6, Ds}, {6, G }, {6, E }, {6, Gs}, {6, F }, {6, A }, {6, Fs}, {6, As}, {6, G }, {6, B }, {6, Gs} },
    { {5, Fs}, {5, As}, {5, G }, {5, B }, {5, Gs}, {6, C }, {5, A }, {6, Cs}, {5, As}, {6, D }, {5, B }, {6, Ds}, {6, C }, {6, E }, {6, Cs} },
    { {4, B }, {5, Ds}, {5, C }, {5, E }, {5, Cs}, {5, F }, {5, D }, {5, Fs}, {5, Ds}, {5, G }, {5, E }, {5, Gs}, {5, F }, {5, A }, {5, Fs} },
    { {4, E }, {4, Gs}, {4, F }, {4, A }, {4, Fs}, {4, As}, {4, G }, {4, B }, {4, Gs}, {5, C }, {4, A }, {5, Cs}, {4, As}, {5, D }, {4, B } },
    { {3, A }, {4, Cs}, {3, As}, {4, D }, {3, B }, {4, Ds}, {4, C }, {4, E }, {4, Cs}, {4, F }, {4, D }, {4, Fs}, {4, Ds}, {4, G }, {4, E } },
    { {3, D }, {3, Fs}, {3, Ds}, {3, G }, {3, E }, {3, Gs}, {3, F }, {3, A }, {3, Fs}, {3, As}, {3, G }, {3, B }, {3, Gs}, {4, C }, {3, A } },
    { {2, G }, {2, B }, {2, Gs}, {3, C }, {2, A }, {3, Cs}, {2, As}, {3, D }, {2, B }, {3, Ds}, {3, C }, {3, E }, {3, Cs}, {3, F }, {3, D } },
    { {     }, {2, E }, {     }, {2, F }, {     }, {2, Fs}, {     }, {2, G }, {     }, {2, Gs}, {     }, {2, A }, {     }, {2, As}, {     } }
};