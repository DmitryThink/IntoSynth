/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
ThinkSynthProcessorEditor::ThinkSynthProcessorEditor (ThinkSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor(p), oscGui(p), envGui(p), filterGui(p), reverbGui(p), distortionGUI(p), delayGUI(p), keyboardComponent (p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (600, 500);
    
    addAndMakeVisible(&oscGui);
    addAndMakeVisible(&envGui);
    addAndMakeVisible(&filterGui);
    addAndMakeVisible(&reverbGui);
    addAndMakeVisible(&distortionGUI);
    addAndMakeVisible(&delayGUI);
    addAndMakeVisible(keyboardComponent);
}

ThinkSynthProcessorEditor::~ThinkSynthProcessorEditor()
{
}

//==============================================================================
void ThinkSynthProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);

}

void ThinkSynthProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    
    const int componentWidth = 200;
    const int componentHeight = 200;
    
    oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

    distortionGUI.setBounds(0, 200, 200, 200);
    reverbGui.setBounds(200, 200, 200, 200);
    delayGUI.setBounds(400, 200, 200, 200);
    keyboardComponent.setBounds (0, getHeight()-100, getWidth(), 100);
}


