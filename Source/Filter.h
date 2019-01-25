/*
  ==============================================================================

    Filter.h
    Created: 23 Jan 2019 2:47:50am
    Author:  think

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Filter
{
 public:
  Filter();
  ~Filter();
  void setGUI(AudioProcessorValueTreeState& tree);
  void prepareToPlay(double sampleRate, int samplesPerBlock, int numberOfOutPutChannels);
  void updateFilter(AudioProcessorValueTreeState& tree, double sampleRate);
  void process(dsp::AudioBlock<float>& block);

 private:
  dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float> , dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
};
