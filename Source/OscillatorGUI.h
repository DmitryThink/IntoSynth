/*
  ==============================================================================

    OscillatorGUI.h
    Created: 11 Jan 2018 1:41:05pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class OscillatorGUI    : public Component,
                      private ComboBox::Listener
{
public:
    OscillatorGUI(JuceSynthFrameworkAudioProcessor&);
    ~OscillatorGUI();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox*) override;

private:
    ComboBox oscComboBox;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> oscSelection;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceSynthFrameworkAudioProcessor& processor;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorGUI)
};
