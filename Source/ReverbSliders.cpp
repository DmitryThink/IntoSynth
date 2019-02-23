/*
  ==============================================================================

    Reverb.cpp
    Created: 16 Nov 2018 10:30:23pm
    Author:  think

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbSliders.h"

//==============================================================================
ReverbSliders::ReverbSliders(ThinkSynthAudioProcessor& p) :
        processor(p)
{
    setSize(300, 300);

    //slider initialization values
    roomSizeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    roomSizeSlider.setRange(0.0f, 1.0f);
    roomSizeSlider.setValue(0.5f);
    roomSizeSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&roomSizeSlider);

    dampingSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    dampingSlider.setRange(0.0f, 1.0f);
    dampingSlider.setValue(0.5f);
    dampingSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&dampingSlider);

    wetLevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    wetLevelSlider.setRange(0.0f, 1.0f);
    wetLevelSlider.setValue(0.5f);
    wetLevelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&wetLevelSlider);

    dryLevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    dryLevelSlider.setRange(0.0f, 1.0f);
    dryLevelSlider.setValue(0.5f);
    dryLevelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&dryLevelSlider);

    widthSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    widthSlider.setRange(0.0f, 1.0f);
    widthSlider.setValue(0.5f);
    widthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&widthSlider);

    addAndMakeVisible(&turnButton);

    roomSizeVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "roomSize", roomSizeSlider);
    dampingVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "damping", dampingSlider);
    wetLevelVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "wetLevel", wetLevelSlider);
    dryLevelVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "dryLevel", dryLevelSlider);
    widthVal = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "width", roomSizeSlider);
    turnButtonVal = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "reverbTurn", turnButton);
}

ReverbSliders::~ReverbSliders()
{
}

void ReverbSliders::paint (Graphics& g)
{
    //fancy stuff for the UI background etc
    Rectangle<int> titleArea (0, 10, getWidth(), 20);

    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Reverb", titleArea, Justification::centredTop);

    //static positioning for now due to time, make dynamic later
    g.drawText ("RS", 53, 150, 20, 20, Justification::centredTop);
    g.drawText ("D", 73, 150, 20, 20, Justification::centredTop);
    g.drawText ("W", 93, 150, 20, 20, Justification::centredTop);
    g.drawText ("D", 110, 150, 20, 20, Justification::centredTop);
    g.drawText ("Wth", 128, 150, 20, 20, Justification::centredTop);

    Rectangle <float> area (25, 25, 150, 150);

    g.setColour(Colours::blue);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void ReverbSliders::resized()
{
    //draws the sliders...we use a rectangle object to dynamically size the UI (if we want to resize for IPad etc without needing to change ALL settings
    Rectangle<int> area = getLocalBounds().reduced(50);

    int sliderWidth = 20;
    int sliderHeight = 175;

    //draw sliders by reducing area from rectangle above
    roomSizeSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    dampingSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    wetLevelSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    dryLevelSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    widthSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    turnButton.setBounds (40, 40, 20, 20);
}
