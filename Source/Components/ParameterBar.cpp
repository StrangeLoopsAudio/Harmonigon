/*
  ==============================================================================

    ParameterBar.cpp
    Created: 5 Jul 2020 8:02:47pm
    Author:  brady

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterBar.h"

#define BPM_WIDTH 250
#define KEY_WIDTH 100
#define SCALE_TYPE_WIDTH 80
#define BUTTON_PADDING_WIDTH 10

//==============================================================================
ParameterBar::ParameterBar()
{
    setSize(100, 100);

    sliderBpm.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    sliderBpm.setTextValueSuffix(" bpm");
    sliderBpm.setRange(Range<double>(50, 200), 1);
    sliderBpm.setValue(128, dontSendNotification);
    sliderBpm.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    sliderBpm.setColour(Slider::ColourIds::textBoxHighlightColourId, Colours::purple.darker());
    addAndMakeVisible(sliderBpm);

    /* Add key items */
    for (int i = 0; i < NoteUtils::NUM_KEYS; i++)
    {
        comboKey.addItem(NoteUtils::keyToString((NoteUtils::Key)i), i + 1);
    }
    comboKey.setSelectedItemIndex(0, true);
    addAndMakeVisible(comboKey);

    /* Add scale items */
    comboScaleType.addItem("Major", NoteUtils::MAJOR + 1);
    comboScaleType.addItem("Minor", NoteUtils::MINOR + 1);
    comboScaleType.setSelectedItemIndex(0, true);
    addAndMakeVisible(comboScaleType);

    buttonPlayStop.setButtonText("Play Paths");
    buttonPlayStop.setColour(TextButton::buttonColourId, Colours::green);
    addAndMakeVisible(buttonPlayStop);

    buttonAddPath.setButtonText("Add Path +");
    addAndMakeVisible(buttonAddPath);

    buttonPathMode.setButtonText("Hex Mode");
    addAndMakeVisible(buttonPathMode);
}

ParameterBar::~ParameterBar()
{
}

void ParameterBar::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
}

void ParameterBar::resized()
{
    Rectangle<int> b = getLocalBounds();

    /* Top half */
    Rectangle<int> top = b.removeFromTop(getHeight() / 2);
    sliderBpm.setBounds(top.removeFromLeft(BPM_WIDTH));
    top.removeFromLeft(BUTTON_PADDING_WIDTH);
    comboKey.setBounds(top.removeFromLeft(KEY_WIDTH));
    top.removeFromLeft(BUTTON_PADDING_WIDTH);
    comboScaleType.setBounds(top.removeFromLeft(SCALE_TYPE_WIDTH));
    top.removeFromLeft(BUTTON_PADDING_WIDTH);
    buttonPlayStop.changeWidthToFitText();
    buttonPlayStop.setBounds(top.removeFromLeft(buttonPlayStop.getWidth()));
    top.removeFromLeft(BUTTON_PADDING_WIDTH);
    buttonAddPath.changeWidthToFitText();
    buttonAddPath.setBounds(top.removeFromRight(buttonAddPath.getWidth()));
    buttonPathMode.changeWidthToFitText();
    buttonPathMode.setBounds(top.removeFromRight(buttonPathMode.getWidth()));

}
