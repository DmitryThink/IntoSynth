/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OscillatorGUI.h"
#include "Envelope.h"
#include "Filter.h"
#include "ReverbSliders.h"
#include "DistortionGUI.h"
#include "DelayGUI.h"


//==============================================================================
/**
*/
class JuceSynthFrameworkAudioProcessorEditor  : public AudioProcessorEditor

{
public:
    JuceSynthFrameworkAudioProcessorEditor (JuceSynthFrameworkAudioProcessor&);
    ~JuceSynthFrameworkAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceSynthFrameworkAudioProcessor& processor;
    
    OscillatorGUI oscGui;
    Envelope envGui;
    Filter filterGui;
    ReverbSliders reverbGui;
    DistortionGUI distortionGUI;
    DelayGUI delayGUI;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceSynthFrameworkAudioProcessorEditor)
};
