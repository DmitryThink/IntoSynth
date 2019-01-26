/*
  ==============================================================================

    Delay.h
    Created: 26 Jan 2019 11:56:45am
    Author:  think

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Delay
{
 public:
  Delay();
  ~Delay();
  void setParametersToTree(AudioProcessorValueTreeState &tree);
  void prepareToPlay(double sampleRate, int samplesPerBlock, int numberOfOutPutChannels);
  void processBlock (AudioSampleBuffer& buffer, AudioProcessorValueTreeState &tree, int totalNumOutputChannels);
  void fillDelayBuffer(int channel, const int bufferLenght,
      const int delayBufferLenght, const float* bufferData,
      const float* delayBufferData, AudioProcessorValueTreeState &tree);
  void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLenght,
      const int delayBufferLenght, const float* bufferData,
      const float* delayBufferData, AudioProcessorValueTreeState &tree);
  void feedbackDelay(int channel, const int bufferLenght, const int delayBufferLenght,
      float* dryBuffer, AudioProcessorValueTreeState &tree);

 private:
  AudioSampleBuffer mDelayBuffer;
  int mWritePosition { 0 };
  int mSampleRate { 44100 };
};