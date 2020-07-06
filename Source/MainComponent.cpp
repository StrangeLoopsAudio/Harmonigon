/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#define PARAM_BAR_HEIGHT 100

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1000, 700);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    m_grid.setBounds(0, 100, getWidth(), getHeight() - 100);
    addAndMakeVisible(m_grid);

    m_paramBar.setBounds(0, 0, getWidth(), 100);
    m_paramBar.sliderBpm.addListener(this);
    addAndMakeVisible(m_paramBar);
    m_moveDuration = (1 / m_paramBar.sliderBpm.getValue()) * 60 * 1000;

    startTimer(m_moveDuration);

    m_synth.addSound(new SineWaveSound());
    for (int i = 0; i < 8; i++)
    {
        m_synth.addVoice(new SineWaveVoice());
    }
}

void MainComponent::sliderDragEnded(Slider* slider)
{
    if (slider == &m_paramBar.sliderBpm)
    {
        stopTimer();
        m_moveDuration = (1 / m_paramBar.sliderBpm.getValue()) * 60 * 1000;
        startTimer(m_moveDuration);
    }
}

void MainComponent::timerCallback()
{
    Array<NoteUtils::HexTile> notes = m_grid.getNotesToPlay();
    m_synth.allNotesOff(1, true);
    for (NoteUtils::HexTile note : notes)
    {
        m_synth.noteOn(1, NoteUtils::tileToMidiNote(note), 1);
    }
    m_grid.moveTracers(m_moveDuration);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    m_synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    m_synth.renderNextBlock(*bufferToFill.buffer, MidiBuffer(), bufferToFill.startSample, bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    Rectangle<int> b = getLocalBounds();
    m_paramBar.setBounds(b.removeFromTop(PARAM_BAR_HEIGHT));
    m_grid.setBounds(b);
}
