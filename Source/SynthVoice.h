/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Oct 2017 10:18:29am
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Maximilian/maximilian.h"


    class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    
    
    //=======================================================
    
    void getOscType(float* selection)
    {
        theWave = *selection;
    }

    //=======================================================
    
    double setOscType ()
    {
        switch (theWave){
            case 0:
                return osc1.sinewave(frequency);
            case 1:
                return osc1.saw(frequency);
            case 2:
                return osc1.square(frequency);
            case 3:
                return osc1.noise();
            case 4:
                return osc1.sawn(frequency);
            default:
                return osc1.sinewave(frequency);
        }
    }
    
    //=======================================================
    
    void getEnvelopeParams(float* attack, float* decay, float* sustain, float* release)
    {
        env1.setAttack(*attack);
        env1.setDecay(*decay);
        env1.setSustain(*sustain);
        env1.setRelease(*release);
    }
    
    //=======================================================
    
    double setEnvelope()
    {
        return env1.adsr(setOscType(), env1.trigger);
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
        env1.trigger = 1;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
    }
    
    //=======================================================
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
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
    double level;
    double frequency;
    int theWave;
    
    int filterChoice;
    float cutoff;
    float resonance;
    
    maxiOsc osc1;
    maxiEnv env1;
// distortion;
};
