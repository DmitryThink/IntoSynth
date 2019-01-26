/*
  ==============================================================================

    Delay.cpp
    Created: 26 Jan 2019 11:56:45am
    Author:  think

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay() {}
Delay::~Delay() {}

void Delay::setParametersToTree(AudioProcessorValueTreeState &tree){
  NormalisableRange<float> delayWetVal (0.0f, 1.0f);
  NormalisableRange<float> delayFeedbackVal (0.0f, 1.0f);
  NormalisableRange<float> delayTimeVal (0.0f, 2000.0f);
  NormalisableRange<float> buttonRange (0, 1, 1);
  tree.createAndAddParameter("delayWet", "DelayWet", "delayWet", delayWetVal, 0, nullptr, nullptr);
  tree.createAndAddParameter("delayFeedback", "DelayFeedback", "delayFeedback", delayFeedbackVal, 0, nullptr, nullptr);
  tree.createAndAddParameter("delayTime", "DelayTime", "delayTime", delayTimeVal, 0, nullptr, nullptr);
  tree.createAndAddParameter("delayTurn", "DelayTurn", "delayTurn", buttonRange, 0, nullptr, nullptr);
}

void Delay::prepareToPlay(double sampleRate, int samplesPerBlock, int numberOfOutPutChannels){
  mDelayBuffer.setSize (numberOfOutPutChannels, 2.0 * (samplesPerBlock + sampleRate), false, true);
  mSampleRate = sampleRate;
}


void Delay::processBlock(AudioSampleBuffer& buffer, AudioProcessorValueTreeState &tree, int totalNumOutputChannels){
  if (*tree.getRawParameterValue("delayTurn")) {
    const int bufferLenght = buffer.getNumSamples();
    const int delayBufferLenght = mDelayBuffer.getNumSamples();

    for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
      const float *bufferData = buffer.getReadPointer(channel);
      const float *delayBufferData = mDelayBuffer.getReadPointer(channel);
      float *dryBuffer = buffer.getWritePointer(channel);

      fillDelayBuffer(channel, bufferLenght, delayBufferLenght, bufferData, delayBufferData, tree);
      getFromDelayBuffer(buffer, channel, bufferLenght, delayBufferLenght, bufferData, delayBufferData, tree);
      feedbackDelay(channel, bufferLenght, delayBufferLenght, dryBuffer, tree);
    }
    mWritePosition += bufferLenght;
    mWritePosition %= delayBufferLenght;
  }
}

void Delay::fillDelayBuffer(int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData, AudioProcessorValueTreeState &tree)
{
  double wet = *tree.getRawParameterValue("delayWet");
  if (delayBufferLenght > bufferLenght + mWritePosition){
    mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLenght, wet, wet);
  } else {
    const int bufferRemaining = delayBufferLenght - mWritePosition;

    mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, wet, wet);
    mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLenght - bufferRemaining, wet, wet);
  }
}

void Delay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData, AudioProcessorValueTreeState &tree){
  int delayTime = *tree.getRawParameterValue("delayTime");
  const int readPosition = (delayBufferLenght + mWritePosition - (mSampleRate * delayTime/1000)) % delayBufferLenght;

  if(delayBufferLenght > bufferLenght + readPosition) {
    buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLenght);
  }else{
    const int bufferRemaining = delayBufferLenght - readPosition;
    buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
    buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLenght - bufferRemaining);
  }
}
void Delay::feedbackDelay(int channel, const int bufferLenght,
                                             const int delayBufferLenght, float* dryBuffer, AudioProcessorValueTreeState &tree){
  double feedback = *tree.getRawParameterValue("delayFeedback");
  if (delayBufferLenght > bufferLenght + mWritePosition){
    mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLenght, feedback, feedback);
  }else{
    const int bufferRemaining = delayBufferLenght - mWritePosition;
    mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, feedback, feedback);
    mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLenght - bufferRemaining, feedback, feedback);
  }
}