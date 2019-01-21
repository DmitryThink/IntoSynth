/*
  ==============================================================================

    Envelope.h
    Created: 11 Jan 2018 2:31:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class EnvelopeGUI    : public Component
{
public:
    EnvelopeGUI(ThinkSynthAudioProcessor&);
    ~EnvelopeGUI();
    void paint (Graphics&) override;
    void resized() override;

private:
    Slider attack;
    Slider decay;
    Slider sustain;
    Slider release;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> envelopeAttack;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> envelopeDecay;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> envelopeSustain;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> envelopeRelease;

    ThinkSynthAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeGUI)
};
