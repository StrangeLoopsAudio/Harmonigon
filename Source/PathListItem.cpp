/*
  ==============================================================================

    PathListItem.cpp
    Created: 21 Jul 2020 4:43:58pm
    Author:  Nick Merfeld

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PathListItem.h"

//==============================================================================
OtherLookAndFeel::OtherLookAndFeel()
{
    
}

juce::Font OtherLookAndFeel::getComboBoxFont(ComboBox &)
{
    return 10.0f;
}


PathListItem::PathListItem(int id): id(id)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(300, 100);
    setLookAndFeel(&otherLookAndFeel);
    
    repeatType.addItem("Start From Beginning", 1);
    repeatType.addItem("Travel Back Down Path", 2);
    repeatType.setSelectedItemIndex(0, true);
    addAndMakeVisible(repeatType);
    
    stepIntervalType.addItem("1/4", 1);
    stepIntervalType.addItem("1/8", 2);
    stepIntervalType.addItem("1/16", 3);
    stepIntervalType.setSelectedItemIndex(0, true);
    addAndMakeVisible(stepIntervalType);
    
    loopLength.addItem("1", 1);
    loopLength.addItem("2", 2);
    loopLength.addItem("3", 3);
    loopLength.addItem("4", 4);
    loopLength.setSelectedItemIndex(0, true);
    addAndMakeVisible(loopLength);
    
    name.setText("Path " + String(id + 1), dontSendNotification);
    name.setJustificationType(Justification::centred);
    addAndMakeVisible(name);
    
    repeatTypeLabel.setText("Repeat Type", dontSendNotification);
    addAndMakeVisible(repeatTypeLabel);

    stepIntervalTypeLabel.setText("Note Type", dontSendNotification);
    addAndMakeVisible(stepIntervalTypeLabel);

    loopLengthLabel.setText("Loop Length", dontSendNotification);
    addAndMakeVisible(loopLengthLabel);
    
}

PathListItem::~PathListItem()
{
    setLookAndFeel (nullptr);
}

void PathListItem::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    g.drawLine(getWidth() / 3, 0, getWidth() / 3, getHeight());
    g.drawLine(0, getHeight() / 2, getWidth() / 3, getHeight() / 2);

    g.drawLine(getWidth() / 3, getHeight() / 3, 2 * (getWidth() / 3), getHeight() / 3);
    g.drawLine(getWidth() / 3, (2 * getHeight()) / 3, 2 * (getWidth() / 3), (2 * getHeight()) / 3);

}

void PathListItem::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> b = getLocalBounds();
    int panelHeightThird = b.getHeight() / 3;
    int panelWidthThird = b.getWidth() / 3;

    Rectangle<int> optionBoxes = b.removeFromRight(panelWidthThird);
    Rectangle<int> labelOptions = b.removeFromRight(panelWidthThird);
    
    repeatType.setBounds(optionBoxes.removeFromTop(panelHeightThird));
    stepIntervalType.setBounds(optionBoxes.removeFromTop(panelHeightThird));
    loopLength.setBounds(optionBoxes.removeFromTop(panelHeightThird));
    
    name.setBounds(b.removeFromTop(b.getHeight() / 2));
    
    repeatTypeLabel.setBounds(labelOptions.removeFromTop(panelHeightThird));
    stepIntervalTypeLabel.setBounds(labelOptions.removeFromTop(panelHeightThird));
    loopLengthLabel.setBounds(labelOptions.removeFromTop(panelHeightThird));
}
