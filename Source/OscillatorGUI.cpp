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
OscillatorGUI::OscillatorGUI(JuceSynthFrameworkAudioProcessor& p) :
processor(p)
{
    setSize(200, 200);
    
    oscMenu.addItem("Sine", 1);
    oscMenu.addItem("Saw", 2);
    oscMenu.addItem("Square", 3);
    oscMenu.addItem("Noise", 4);
    oscMenu.addItem("Sawn", 5);
    oscMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&oscMenu);
    oscMenu.addListener(this);

    oscSelection = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "wavetype", oscMenu);
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
    oscMenu.setBounds(area.removeFromTop(20));
}

void OscillatorGUI::comboBoxChanged(ComboBox* box)
{
    
}

