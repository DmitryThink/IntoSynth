/*
  ==============================================================================

    Reverb.h
    Created: 16 Nov 2018 10:30:23pm
    Author:  think

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ReverbSliders    : public Component
{
public:
    ReverbSliders(JuceSynthFrameworkAudioProcessor&);
    ~ReverbSliders();

    void paint (Graphics&) override;
    void resized() override;

private:
    Slider roomSizeSlider;
    Slider dampingSlider;
    Slider wetLevelSlider;
    Slider dryLevelSlider;
    Slider widthSlider;
    ToggleButton turnButton;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> roomSizeVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dampingVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> wetLevelVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dryLevelVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> widthVal;
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> turnButtonVal;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceSynthFrameworkAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbSliders)
};
