/*
  ==============================================================================

    Synth.h
    Created: 2 Jul 2020 9:36:28pm
    Author:  brady

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define PATH_NOTES_CHANNEL 1
#define FREE_PLAY_CHANNEL 2

struct SineWaveSound : public SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class SineWaveVoice : public SynthesiserVoice
{
public:
    SineWaveVoice() {}

    bool canPlaySound(SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void stopNote(float /*velocity*/, bool allowTailOff) override;
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};
