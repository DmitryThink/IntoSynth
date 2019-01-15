/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
ThinkSynthAudioProcessor::ThinkSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
tree(*this, nullptr)
#endif
{
    dist = new Distortion();
    //need these normalisable range objects for the tree state below this
    NormalisableRange<float> attackParam (0.1f, 5000.0f);
    NormalisableRange<float> decayParam (1.0f, 2000.0f);
    NormalisableRange<float> sustainParam (0.0f, 1.0f);
    NormalisableRange<float> releaseParam (0.1f, 5000.0f);
    
    //params that make it possible to set/get states and automate parameters in your DAW.  Also connects values between the slider and the values here
    tree.createAndAddParameter("attack", "Attack", "attack", attackParam, 0.1f, nullptr, nullptr);
    tree.createAndAddParameter("decay", "Decay", "decay", decayParam, 1.0f, nullptr, nullptr);
    tree.createAndAddParameter("sustain", "Sustain", "sustain", sustainParam, 0.8f, nullptr, nullptr);
    tree.createAndAddParameter("release", "Release", "release", releaseParam, 0.1f, nullptr, nullptr);

    NormalisableRange<float> wavetypeParam (0, 2);
    tree.createAndAddParameter("wavetype", "WaveType", "wavetype", wavetypeParam, 0, nullptr, nullptr);
    
    NormalisableRange<float> filterTypeVal (0, 2);
    NormalisableRange<float> filterVal (20.0f, 10000.0f);
    NormalisableRange<float> resVal (1, 5);
    tree.createAndAddParameter("filterType", "FilterType", "filterType", filterTypeVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("filterCutoff", "FilterCutoff", "filterCutoff", filterVal, 400.0f, nullptr, nullptr);
    tree.createAndAddParameter("filterRes", "FilterRes", "filterRes", resVal, 1, nullptr, nullptr);

    NormalisableRange<float> roomSizeVal (0.0f, 1.0f);
    NormalisableRange<float> dryLevelVal (0.0f, 1.0f);
    NormalisableRange<float> wetLevelVal (0.0f, 1.0f);
    NormalisableRange<float> dampingVal (0.0f, 1.0f);
    NormalisableRange<float> widthVal (0.0f, 1.0f);
    NormalisableRange<float> buttonRange (0, 1, 1);
    tree.createAndAddParameter("roomSize", "RoomSize", "roomSize", roomSizeVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("dryLevel", "DryLevel", "dryLevel", dryLevelVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("wetLevel", "WetLevel", "wetLevel", wetLevelVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("damping", "Damping", "damping", dampingVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("width", "Width", "width", widthVal, 0, nullptr, nullptr);
    tree.createAndAddParameter("turn", "Turn", "turn", buttonRange, 0, nullptr, nullptr);

    NormalisableRange<float> distortionType (0, 7);
    NormalisableRange<float> distortionAmplitude (0.0, 100.f);
    NormalisableRange<float> distortionRatio (0, 1);
    tree.createAndAddParameter("distortionType", "DistortionType", "distortionType", distortionType, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionAmplitude", "DistortionAmplitude", "distortionAmplitude", distortionAmplitude, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionRatio", "DistortionRatio", "distortionRatio", distortionRatio, 0, nullptr, nullptr);

    mySynth.clearVoices();
    
    for (int i = 0; i < 5; i++)
    {
        mySynth.addVoice(new SynthVoice());
    }
    mySynth.clearSounds();
    mySynth.addSound(new SynthSound());
}

ThinkSynthAudioProcessor::~ThinkSynthAudioProcessor()
{
}

//==============================================================================
const String ThinkSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThinkSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThinkSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThinkSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThinkSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThinkSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThinkSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThinkSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String ThinkSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void ThinkSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ThinkSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);
    theReverb.setSampleRate(lastSampleRate);
    
    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    stateVariableFilter.reset();
    stateVariableFilter.prepare(spec);
    keyboardState.reset();
    updateFilter();

    mSampleRate = sampleRate;
    const int totalNumInputChannels  = getTotalNumOutputChannels();

    // sample buffer for 2 seconds + 2 buffers safety
    mDelayBuffer.setSize (totalNumInputChannels, 2.0 * (samplesPerBlock + sampleRate), false, true);
}

void ThinkSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThinkSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ThinkSynthAudioProcessor::updateFilter()
{
    int menuChoice = *tree.getRawParameterValue("filterType");
    int freq = *tree.getRawParameterValue("filterCutoff");
    int res = *tree.getRawParameterValue("filterRes");
    
    if (menuChoice == 0)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }
    
    if (menuChoice == 1)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }
    
    if (menuChoice == 2)
    {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }
}

void ThinkSynthAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    //get the voice and get the params needed to make the voice
    for (int i = 0; i < mySynth.getNumVoices(); i++)
    {
        //if myVoice sucessfully casts as a SynthVoice*, get the voice and set the params
        if ((myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i))))
        {
            myVoice->getEnvelopeParams(tree.getRawParameterValue("attack"),
                                       tree.getRawParameterValue("decay"),
                                       tree.getRawParameterValue("sustain"),
                                       tree.getRawParameterValue("release"));
            
            myVoice->getOscType(tree.getRawParameterValue("wavetype"));
            
            myVoice->getFilterParams(tree.getRawParameterValue("filterType"),
                                     tree.getRawParameterValue("filterCutoff"),
                                     tree.getRawParameterValue("filterRes"));

        }
    }

    keyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    buffer.clear();
    for(auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      buffer.clear(i, 0, buffer.getNumChannels());

    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    updateFilter();

    dsp::AudioBlock<float> block (buffer);

    // Distortion
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float* const samples = buffer.getWritePointer(0);
        //std::cout << *tree.getRawParameterValue("distortionType") << " " << *tree.getRawParameterValue("distortionAmplitude") << " " << *tree.getRawParameterValue("distortionRatio") << std::endl;
        samples[i] = dist->processSample(samples[i], *tree.getRawParameterValue("distortionType"), *tree.getRawParameterValue("distortionAmplitude"), *tree.getRawParameterValue("distortionRatio"));
    }

    // Delay
    const int bufferLenght = buffer.getNumSamples();
    const int delayBufferLenght = mDelayBuffer.getNumSamples();

    for(int channel = 0; channel < totalNumOutputChannels; ++channel){
      const float* bufferData = buffer.getReadPointer(channel);
      const float* delayBufferData = mDelayBuffer.getReadPointer(channel);

      fillDelayBuffer(channel, bufferLenght, delayBufferLenght, bufferData, delayBufferData);
      getFromDelayBuffer(buffer, channel, bufferLenght, delayBufferLenght, bufferData, delayBufferData);
    }
    mWritePosition += bufferLenght;
    mWritePosition %= delayBufferLenght;

    // Reverb
    if (*tree.getRawParameterValue("turn")) {
        theReverbParameters.roomSize = *tree.getRawParameterValue("roomSize");
        theReverbParameters.dryLevel = *tree.getRawParameterValue("dryLevel");
        theReverbParameters.wetLevel = *tree.getRawParameterValue("wetLevel");
        theReverbParameters.damping = *tree.getRawParameterValue("damping");
        theReverbParameters.width = *tree.getRawParameterValue("width");
        theReverb.setParameters(theReverbParameters);
        if (getTotalNumOutputChannels() == 1)
            theReverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
        else
            theReverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }

    // Filter
    stateVariableFilter.process(dsp::ProcessContextReplacing<float> (block));
}

void ThinkSynthAudioProcessor::fillDelayBuffer(int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData)
{
    if (delayBufferLenght > bufferLenght + mWritePosition){
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLenght, 0.8, 0.8);
    } else {
        const int bufferRemaining = delayBufferLenght - mWritePosition;

        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, 0.8, 0.8);
        mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLenght - bufferRemaining, 0.8, 0.8);
    }
}

void ThinkSynthAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLenght, const int delayBufferLenght, const float* bufferData, const float* delayBufferData){
    int delayTime = 600;
    const int readPosition = (delayBufferLenght + mWritePosition - (mSampleRate * delayTime/1000)) % delayBufferLenght;

    if(delayBufferLenght > bufferLenght + readPosition) {
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLenght);
    }else{
        const int bufferRemaining = delayBufferLenght - readPosition;
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLenght - bufferRemaining);
    }
}

//==============================================================================
bool ThinkSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ThinkSynthAudioProcessor::createEditor()
{
    return new ThinkSynthProcessorEditor (*this);
}

//==============================================================================
void ThinkSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThinkSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThinkSynthAudioProcessor();
}
