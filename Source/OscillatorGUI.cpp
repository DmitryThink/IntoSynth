/*
  ==============================================================================

    OscillatorGUI.cpp
    Created: 11 Jan 2018 1:41:05pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorGUI.h"

//==============================================================================
OscillatorGUI::OscillatorGUI(ThinkSynthAudioProcessor& p) :
processor(p)
{
    setSize(200, 200);
    
    oscComboBox.addItem("Sine", 1);
    oscComboBox.addItem("Saw", 2);
    oscComboBox.addItem("Square", 3);
    oscComboBox.addItem("Noise", 4);
    oscComboBox.addItem("Sawn", 5);
    oscComboBox.setJustificationType(Justification::centred);
    addAndMakeVisible(&oscComboBox);
    oscComboBox.addListener(this);

    oscSelection = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "wavetype", oscComboBox);
}

OscillatorGUI::~OscillatorGUI()
{
}

void OscillatorGUI::paint (Graphics& g)
{
    //background stuff
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("OscillatorGUI", titleArea, Justification::centredTop);
    
    Rectangle <float> area (25, 25, 150, 150);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void OscillatorGUI::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40);
    oscComboBox.setBounds(area.removeFromTop(20));
}

void OscillatorGUI::comboBoxChanged(ComboBox* box)
{
    
}

