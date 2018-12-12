/*
  ==============================================================================

    DistortionGUI.cpp
    Created: 11 Dec 2018 3:49:53pm
    Author:  think

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DistortionGUI.h"
//==============================================================================
DistortionGUI::DistortionGUI(JuceSynthFrameworkAudioProcessor& p) :
        processor(p)
{
    setSize(200, 200);

    distortionMenu.addItem("Soft Clip", 1);
    distortionMenu.addItem("Arctangent", 2);
    distortionMenu.addItem("Hard Clip", 3);
    distortionMenu.addItem("Square Law", 4);
    distortionMenu.addItem("Cubic WaveShaper", 5);
    distortionMenu.addItem("Foldback", 6);
    distortionMenu.addItem("Gloubi Approx", 7);
    distortionMenu.addItem("Gloubi Boulga", 8);
    
    distortionMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&distortionMenu);
    distortionTypeVal = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "distortionType", distortionMenu);

    distortionAmplitude.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    distortionAmplitude.setRange(0., 1000.0);
    distortionAmplitude.setValue (0.);
    distortionAmplitude.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&distortionAmplitude);
    distortionVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionAmplitude", distortionAmplitude);
    distortionAmplitude.setSkewFactorFromMidPoint(1000.0);

    distortionRatio.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    distortionRatio.setRange(0., 1);
    distortionRatio.setValue(0);
    distortionRatio.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&distortionRatio);
    ratioVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionRatio", distortionRatio);
}

DistortionGUI::~DistortionGUI()
{
}

void DistortionGUI::paint (Graphics& g)
{
    //background ui stuff
    Rectangle<int> titleArea (0, 10, getWidth(), 20);

    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("distortion", titleArea, Justification::centredTop);

    Rectangle <float> area (25, 25, 150, 150);

    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void DistortionGUI::resized()
{
    //need to come back and dynamically set these...ok for now
    Rectangle<int> area = getLocalBounds().reduced(40);

    distortionMenu.setBounds(area.removeFromTop(20));
    distortionAmplitude.setBounds (30, 100, 70, 70);
    distortionRatio.setBounds (100, 100, 70, 70);
}
