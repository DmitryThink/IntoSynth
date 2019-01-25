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
#include "EnvelopeGUI.h"
#include "FilterGUI.h"
#include "ReverbSliders.h"
#include "DistortionGUI.h"
#include "DelayGUI.h"


//==============================================================================
/**
*/
class ThinkSynthProcessorEditor  : public AudioProcessorEditor

{
public:
    ThinkSynthProcessorEditor (ThinkSynthAudioProcessor&);
    ~ThinkSynthProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    

private:
    ThinkSynthAudioProcessor& processor;
    
    OscillatorGUI oscGui;
    EnvelopeGUI envGui;
    FilterGUI filterGui;
    ReverbSliders reverbGui;
    DistortionGUI distortionGUI;
    DelayGUI delayGUI;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThinkSynthProcessorEditor)
};
