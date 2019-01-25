/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThinkSynthVoice.h"
#include "ThinkSynthSound.h"
#include "Distortion.h"
#include "Filter.h"

//==============================================================================
/**
*/
class ThinkSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ThinkSynthAudioProcessor();
    ~ThinkSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
//    void updateFilter();

    void fillDelayBuffer(int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData);
    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData);
    void feedbackDelay(int channel, const int bufferLenght, const int delayBufferLenght, float* dryBuffer);

    AudioProcessorValueTreeState tree;

    MidiKeyboardState keyboardState;

private:
    Synthesiser mySynth;
    ThinkSynthVoice* myVoice;

    Reverb theReverb;
    Reverb::Parameters theReverbParameters;

    ScopedPointer<Distortion> dist;
    Filter filter;

    double lastSampleRate;
    int mSampleRate { 44100 };

    AudioSampleBuffer mDelayBuffer;
    int mWritePosition { 0 };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThinkSynthAudioProcessor)
};
