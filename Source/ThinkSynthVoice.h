/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Oct 2017 10:18:29am
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThinkSynthSound.h"
#include "Maximilian/maximilian.h"


    class ThinkSynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <ThinkSynthSound*>(sound) != nullptr;
    }
    
    
    //=======================================================
    
    void getOscType(float* selection)
    {
        oscillatorType = *selection;
    }

    //=======================================================
    
    double setOscType ()
    {
        switch (oscillatorType){
            case 0:
                return oscillator.sinewave(frequency);
            case 1:
                return oscillator.saw(frequency);
            case 2:
                return oscillator.square(frequency);
            case 3:
                return oscillator.noise();
            case 4:
                return oscillator.sawn(frequency);
            default:
                return oscillator.sinewave(frequency);
        }
    }
    
    //=======================================================
    
    void getEnvelopeParams(float* attack, float* decay, float* sustain, float* release)
    {
        envelope.setAttack(*attack);
        envelope.setDecay(*decay);
        envelope.setSustain(*sustain);
        envelope.setRelease(*release);
    }
    
    //=======================================================
    
    double setEnvelope()
    {
        return envelope.adsr(setOscType(), envelope.trigger);
    }
    
    //=======================================================
    
    
    void getFilterParams (float* filterType, float* filterCutoff, float* filterRes)
    {
        filterChoice = *filterType;
        cutoff = *filterCutoff;
        resonance = *filterRes;
    }
    
    //=======================================================
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        envelope.trigger = 1;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
    }
    
    //=======================================================
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        envelope.trigger = 0;
        allowTailOff = true;
        
        if (velocity == 0)
            clearCurrentNote();
    }
    
    //=======================================================
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    //=======================================================
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    //=======================================================
    
    void renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples) override
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, setEnvelope() * 0.3f);
            }
            ++startSample;
        }
    }
    
    //=======================================================
private:
    maxiOsc oscillator;
    int oscillatorType;
    maxiEnv envelope;

    double level;
    double frequency;

    int filterChoice;
    float cutoff;
    float resonance;
};
