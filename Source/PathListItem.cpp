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


PathListItem::PathListItem(HarmonigonPath* path): m_path(path)
{
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

    name.setText("Path " + String(m_path->id + 1), dontSendNotification);
    name.setJustificationType(Justification::centred);
    addAndMakeVisible(name);

    repeatTypeLabel.setText("Repeat Type", dontSendNotification);
    addAndMakeVisible(repeatTypeLabel);

    stepIntervalTypeLabel.setText("Note Type", dontSendNotification);
    addAndMakeVisible(stepIntervalTypeLabel);

    loopLengthLabel.setText("Loop Length", dontSendNotification);
    addAndMakeVisible(loopLengthLabel);
    
    stepIntervalType.addListener(this);
}

PathListItem::~PathListItem()
{
    setLookAndFeel (nullptr);
}

void PathListItem::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    
    /* Dividers */
    g.drawLine(getWidth() / 3, 0, getWidth() / 3, getHeight());
    g.drawLine(0, getHeight() / 2, getWidth() / 3, getHeight() / 2);
    g.drawLine(getWidth() / 3, getHeight() / 3, 2 * (getWidth() / 3), getHeight() / 3);
    g.drawLine(getWidth() / 3, (2 * getHeight()) / 3, 2 * (getWidth() / 3), (2 * getHeight()) / 3);

    /* Color square */
    g.setColour(m_path->colour);
    Rectangle<float> colorSquare(0, 0, 10, 10);
    colorSquare.setCentre(name.getPosition().translated(15, name.getHeight() / 2).toFloat());
    g.fillRoundedRectangle(colorSquare, 3.f);

}

void PathListItem::resized()
{
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

void PathListItem::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &stepIntervalType)
    {
        // 1/4 = 0, 1/8 = 1, 1/16 = 2
        noteType = (int)(comboBoxThatHasChanged->getSelectedId() - 1);

    }
}
