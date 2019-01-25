/*
  ==============================================================================

    Filter.cpp
    Created: 14 Jan 2018 8:59:36pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterGUI.h"

//==============================================================================
FilterGUI::FilterGUI(ThinkSynthAudioProcessor& p) :
processor(p)
{
    setSize(200, 200);

    menu.addItem("Low Pass", 1);
    menu.addItem("High Pass", 2);
    menu.addItem("Band Pass", 3);
    menu.setJustificationType(Justification::centred);
    addAndMakeVisible(&menu);
    filterMenu = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "filterMenu", menu);

    cutoff.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    cutoff.setRange(20.0, 10000.0);
    cutoff.setValue (400.0);
    cutoff.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&cutoff);
    filterCutoff = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "filterCutoff", cutoff);
    cutoff.setSkewFactorFromMidPoint(1000.0);

    res.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    res.setRange(1, 5);
    res.setValue(1);
    res.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&res);
    filterRes = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "filterRes", res);
}

FilterGUI::~FilterGUI()
{
}

void FilterGUI::paint (Graphics& g)
{
    Rectangle<int> titleArea (0, 10, getWidth(), 20);

    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Filter", titleArea, Justification::centredTop);

    Rectangle <float> area (25, 25, 150, 150);

    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void FilterGUI::resized()
{
    //need to come back and dynamically set these...ok for now
    Rectangle<int> area = getLocalBounds().reduced(40);

    menu.setBounds(area.removeFromTop(20));
    cutoff.setBounds (30, 100, 70, 70);
    res.setBounds (100, 100, 70, 70);
}
