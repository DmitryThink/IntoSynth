/*
  ==============================================================================

    DelayGUI.cpp
    Created: 11 Dec 2018 3:49:53pm
    Author:  think

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayGUI.h"
//==============================================================================
DelayGUI::DelayGUI(ThinkSynthAudioProcessor& p) :
    processor(p)
{
  setSize(200, 200);

  time.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  time.setRange(0., 100.0);
  time.setValue (0.);
  time.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(&time);
  delayTime = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "delayTime", time);
  time.setSkewFactorFromMidPoint(1000.0);

  feedback.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  feedback.setRange(0., 1);
  feedback.setValue(0);
  feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(&feedback);
  delayFeedback = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "delayFeedback", feedback);

  wet.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  wet.setRange(0., 1);
  wet.setValue(0);
  wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(&wet);
  delayWet = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "delayWet", wet);

  addAndMakeVisible(&turn);
  delayTurn = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "delayTurn", turn);
}

DelayGUI::~DelayGUI()
{
}

void DelayGUI::paint (Graphics& g)
{
  //background ui stuff
  Rectangle<int> titleArea (0, 10, getWidth(), 20);

  g.fillAll (Colours::black);
  g.setColour(Colours::white);
  g.drawText("Delay", titleArea, Justification::centredTop);

  g.drawText ("Time", 40, 150, 50, 20, Justification::centredTop);
  g.drawText ("Feedback", 100, 150, 70, 20, Justification::centredTop);
  g.drawText ("Wet", 65, 90, 70, 70, Justification::centredTop);

  Rectangle <float> area (25, 25, 150, 150);

  g.setColour(Colours::yellow);
  g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void DelayGUI::resized()
{
  //need to come back and dynamically set these...ok for now
  Rectangle<int> area = getLocalBounds().reduced(40);

  time.setBounds (30, 90, 70, 70);
  feedback.setBounds (100, 90, 70, 70);
  wet.setBounds (65, 30, 70, 70);

  turn.setBounds (40, 40, 20, 20);
}
