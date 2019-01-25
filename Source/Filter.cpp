/*
  ==============================================================================

    Filter.cpp
    Created: 23 Jan 2019 2:47:50am
    Author:  think

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter() {}
Filter::~Filter() {}

void Filter::setGUI(AudioProcessorValueTreeState& tree){
  NormalisableRange<float> filterMenu (0, 2);
  NormalisableRange<float> filterCutoff (20.0f, 10000.0f);
  NormalisableRange<float> filterRes (1, 5);
  tree.createAndAddParameter("filterMenu", "FilterMenu", "filterMenu", filterMenu, 0, nullptr, nullptr);
  tree.createAndAddParameter("filterCutoff", "FilterCutoff", "filterCutoff", filterCutoff, 400.0f, nullptr, nullptr);
  tree.createAndAddParameter("filterRes", "FilterRes", "filterRes", filterRes, 1, nullptr, nullptr);
}

void Filter::prepareToPlay(double sampleRate, int samplesPerBlock, int numberOfOutPutChannels){
  dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = numberOfOutPutChannels;
  stateVariableFilter.reset();
  stateVariableFilter.prepare(spec);
}

void Filter::updateFilter(AudioProcessorValueTreeState& tree, double sampleRate){
  int menuChoice = *tree.getRawParameterValue("filterMenu");
  int freq = *tree.getRawParameterValue("filterCutoff");
  int res = *tree.getRawParameterValue("filterRes");

  switch (menuChoice) {
    case 0:
      stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
      break;
    case 1:
      stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
      break;
    case 2:
      stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
      break;
  }
  stateVariableFilter.state->setCutOffFrequency(sampleRate, freq, res);
}

void Filter::process(dsp::AudioBlock<float>& block){
  stateVariableFilter.process(dsp::ProcessContextReplacing<float> (block));
}