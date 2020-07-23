/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "ParameterBar.h"
#include "PathListPanel.h"
#include "PathListItem.h"
#include "HexGrid.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Synth.h"

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

    void sliderValueChanged(Slider* slider) override {};
    void sliderDragStarted(Slider* slider) override {};
    void sliderDragEnded(Slider* slider) override;
    
    // Inherited via Listener
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(Button* button) override;
    
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    ParameterBar m_paramBar;
    PathListPanel m_pathListPanel;
    double m_moveDuration;
    NoteUtils::ScaleType m_curScaleType;
    NoteUtils::Key m_curKey;

    HexGrid m_grid;
    Synthesiser m_synth;
    int numPaths;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)


};
