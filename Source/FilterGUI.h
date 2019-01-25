/*
  ==============================================================================

    Filter.h
    Created: 14 Jan 2018 8:59:36pm
    Author:  think

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class FilterGUI    : public Component
{
public:
    FilterGUI(ThinkSynthAudioProcessor&);
    ~FilterGUI();

    void paint (Graphics&) override;
    void resized() override;

private:
    Slider cutoff;
    Slider res;
    ComboBox menu;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoff;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterRes;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterMenu;

    ThinkSynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGUI)
};
