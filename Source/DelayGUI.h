/*
  ==============================================================================

    DelayGUI.h
    Created: 11 Dec 2018 3:49:53pm
    Author:  think

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class DelayGUI    : public Component
{
 public:
  DelayGUI(ThinkSynthAudioProcessor&);
  ~DelayGUI();

  void paint (Graphics&) override;
  void resized() override;

 private:
  Slider time;
  Slider feedback;
  Slider wet;
  ToggleButton turn;

  ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> delayTime;
  ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> delayFeedback;
  ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> delayWet;
  ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> delayTurn;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ThinkSynthAudioProcessor& processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayGUI)
};
