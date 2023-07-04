/*
  ==============================================================================

    NoteUtils.h
    Created: 14 Jun 2020 9:00:21pm
    Author:  brady

  ==============================================================================
*/

#pragma once
#include <tuple>
#include <JuceHeader.h>
using namespace std;

#define NUM_ROWS 8
#define NUM_COLS 15

class NoteUtils
{
public:
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
    B,
    NUM_KEYS
};

enum ScaleType
{
    MAJOR,
    MINOR
};

typedef struct _hexTile
{
    int octave;
    Key key;
} HexTile;

static const float freqs[5][12];

static const HexTile hexagons[NUM_ROWS][NUM_COLS];

static const int scaleIntervals[2][6];

static String keyToString(Key key);

static float hexToFreq(HexTile hex);

static int tileToMidiNote(NoteUtils::HexTile tile);

static bool isNoteInKey(Key note, Key key, ScaleType type);

};
