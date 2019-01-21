/*
  ==============================================================================

    Envelope.cpp
    Created: 11 Jan 2018 2:31:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGUI.h"

//==============================================================================
EnvelopeGUI::EnvelopeGUI(ThinkSynthAudioProcessor& p) :
processor(p)
{
    setSize(200, 200);

    attack.setSliderStyle(Slider::SliderStyle::LinearVertical);
    attack.setRange(0.1f, 5000.0f);
    attack.setValue(0.1f);
    attack.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&attack);

    decay.setSliderStyle(Slider::SliderStyle::LinearVertical);
    decay.setRange(1.0f, 2000.0f);
    decay.setValue(1.0f);
    decay.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&decay);

    sustain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    sustain.setRange(0.0f, 1.0f);
    sustain.setValue(0.8f);
    sustain.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sustain);

    release.setSliderStyle(Slider::SliderStyle::LinearVertical);
    release.setRange(0.1f, 5000.0f);
    release.setValue(0.8f);
    release.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&release);

    //sends value of the sliders to the tree state in the processor
    envelopeAttack = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "attack", attack);
    envelopeDecay = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "decay", decay);
    envelopeSustain = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "sustain", sustain);
    envelopeRelease = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "release", release);
}

EnvelopeGUI::~EnvelopeGUI()
{
}

void EnvelopeGUI::paint (Graphics& g)
{
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Envelope", titleArea, Justification::centredTop);

    g.drawText ("A", 53, 150, 20, 20, Justification::centredTop);
    g.drawText ("D", 77, 150, 20, 20, Justification::centredTop);
    g.drawText ("S", 103, 150, 20, 20, Justification::centredTop);
    g.drawText ("R", 128, 150, 20, 20, Justification::centredTop);
    
    Rectangle <float> area (25, 25, 150, 150);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void EnvelopeGUI::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(50);
    
    int sliderWidth = 25;
    int sliderHeight = 175;

    attack.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    decay.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    sustain.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    release.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}



