/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#define PARAM_BAR_HEIGHT 100
#define PATH_LIST_PANEL_WIDTH 300

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1300, 700);

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

    addAndMakeVisible(m_grid);

    m_paramBar.sliderBpm.addListener(this);
    m_paramBar.comboKey.addListener(this);
    m_paramBar.buttonAddPath.addListener(this);
    m_paramBar.buttonPlayStop.addListener(this);
    m_paramBar.buttonPathMode.addListener(this);
    m_curKey = (NoteUtils::Key)(m_paramBar.comboKey.getSelectedId() - 1);
    m_paramBar.comboScaleType.addListener(this);
    m_curScaleType = (NoteUtils::ScaleType)(m_paramBar.comboScaleType.getSelectedId() - 1);
    addAndMakeVisible(m_paramBar);
    m_moveDuration = (1 / m_paramBar.sliderBpm.getValue()) * 60 * 1000;

    addAndMakeVisible(m_pathListPanel);
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

void MainComponent::buttonClicked(Button* button)
{
    if (button == &m_paramBar.buttonAddPath)
    {
        if (m_isAddingPath)
        {
            m_paramBar.buttonAddPath.setButtonText("Add Path +");
            m_paramBar.buttonPathMode.setEnabled(true);
            if (m_isInHexPathMode)
            {
                HarmonigonPath* path = m_grid.getPath();
                if (path->selectedHexes.size() > 0)
                {
                    m_pathListPanel.addPath(path);
                    m_grid.storePath(path);
                }
            }
            else
            {
                HarmonigonPath* path = m_grid.getPath();
                if (path->tracerStart.intType != TracerPoint::INVALID && path->pathDirs.size() > 0)
                {
                    m_pathListPanel.addPath(path);
                    m_grid.storePath(path);
                }
            }
            m_grid.endPath();
        }
        else
        {
            m_grid.startNewPath(m_isInHexPathMode);
            m_paramBar.buttonAddPath.setButtonText("Done Drawing Path :)");
            m_paramBar.buttonPathMode.setEnabled(false);
        }
        m_isAddingPath = !m_isAddingPath;
        m_paramBar.resized();
    }
    else if (button == &m_paramBar.buttonPathMode)
    {
        if (m_isInHexPathMode)
        {
            m_paramBar.buttonPathMode.setButtonText("Line Path Mode");
        }
        else
        {
            m_paramBar.buttonPathMode.setButtonText("Hex Path Mode");
        }
        m_isInHexPathMode = !m_isInHexPathMode;
        m_paramBar.resized();
    }
    else if (button == &m_paramBar.buttonPlayStop)
    {
        if (m_isPlaying)
        {
            m_paramBar.buttonPlayStop.setButtonText("Play");
            m_paramBar.buttonPlayStop.setColour(TextButton::buttonColourId, Colours::green);
        }
        else
        {
            m_paramBar.buttonPlayStop.setButtonText("Stop");
            m_paramBar.buttonPlayStop.setColour(TextButton::buttonColourId, Colours::red);
        }
        m_isPlaying = !m_isPlaying;
        m_paramBar.resized();
    }
}

void MainComponent::timerCallback()
{
    if (m_isPlaying)
    {
        Array<Hexagon *> hexes = m_grid.getNotesToPlay();
        m_synth.allNotesOff(1, true);
        for (Hexagon * hex : hexes)
        {
            NoteUtils::HexTile note = hex->getTile();
            if (NoteUtils::isNoteInKey(note.key, m_curKey, m_curScaleType))
            {
                m_synth.noteOn(1, NoteUtils::tileToMidiNote(note), 1);
                hex->pulse();
            }
        }
        m_grid.moveTracers(m_moveDuration);
    }
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
    m_pathListPanel.setBounds(b.removeFromRight(PATH_LIST_PANEL_WIDTH));
    m_paramBar.setBounds(b.removeFromTop(PARAM_BAR_HEIGHT));
    m_grid.setBounds(b);
}

void MainComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &m_paramBar.comboKey)
    {
        m_curKey = (NoteUtils::Key)(comboBoxThatHasChanged->getSelectedId() - 1);
    }
    else if (comboBoxThatHasChanged == &m_paramBar.comboScaleType)
    {
        m_curScaleType = (NoteUtils::ScaleType)(comboBoxThatHasChanged->getSelectedId() - 1);
    }
}
