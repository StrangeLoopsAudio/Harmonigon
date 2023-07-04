/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "ParameterBar.h"
#include "PathListPanel.h"
#include "HexGrid.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "DSP/Synth.h"

using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent: public AudioAppComponent, Timer, Slider::Listener, ComboBox::Listener, Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void timerCallback() override;

    // Inherited via Listeners
    void sliderValueChanged(Slider* slider) override {};
    void sliderDragStarted(Slider* slider) override {};
    void sliderDragEnded(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(Button* button) override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;


private:
    void onHexButtonPressed(); /* Triggered by hex grid when note or chord is played */
    void onHexButtonReleased(); /* Triggered by hex grid when note or chord is released */
    void checkButtonListeners();
    void turnOffDeletedNotes(HarmonigonPath* path);

    double m_sixteenthNoteDuration;
    NoteUtils::ScaleType m_curScaleType;
    NoteUtils::Key m_curKey;
    bool m_isAddingPath = false;
    bool m_isInHexMode = true;
    bool m_isPlaying = false;

    ParameterBar m_paramBar;
    PathListPanel m_pathListPanel;
    HexGrid m_grid;

    Synthesiser m_synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
