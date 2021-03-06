/*
  ==============================================================================

    DistortionGUI.h
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
class DistortionGUI    : public Component
{
public:
    DistortionGUI(ThinkSynthAudioProcessor&);
    ~DistortionGUI();

    void paint (Graphics&) override;
    void resized() override;

private:
    Slider distortionAmplitude;
    Slider distortionRatio;
    ComboBox distortionMenu;
    ToggleButton turn;

    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> distortionTypeVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> distortionVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> ratioVal;
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> distortionTurn;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ThinkSynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionGUI)
};
