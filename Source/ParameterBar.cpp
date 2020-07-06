/*
  ==============================================================================

    ParameterBar.cpp
    Created: 5 Jul 2020 8:02:47pm
    Author:  brady

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterBar.h"

#define BPM_WIDTH  250
#define BPM_HEIGHT 30

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
}
