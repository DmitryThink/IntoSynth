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

    myVoice->setOscilattorParametersToTree(tree);
    myVoice->setEnvelopeParametersToTree(tree);
    filter.setParametersToTree(tree);

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
    tree.createAndAddParameter("reverbTurn", "ReverbTurn", "turn", buttonRange, 0, nullptr, nullptr);

    delay.setParametersToTree(tree);

    NormalisableRange<float> distortionType (0, 7);
    NormalisableRange<float> distortionAmplitude (0.0, 100.f);
    NormalisableRange<float> distortionRatio (0, 1);
    tree.createAndAddParameter("distortionType", "DistortionType", "distortionType", distortionType, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionAmplitude", "DistortionAmplitude", "distortionAmplitude", distortionAmplitude, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionRatio", "DistortionRatio", "distortionRatio", distortionRatio, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionTurn", "DistortionTurn", "distortionTurn", buttonRange, 0, nullptr, nullptr);

    mySynth.clearVoices();
    
    for (int i = 0; i < 5; i++)
    {
        mySynth.addVoice(new ThinkSynthVoice());
    }
    mySynth.clearSounds();
    mySynth.addSound(new ThinkSynthSound());
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
    reverb.setSampleRate(lastSampleRate);
    filter.prepareToPlay(lastSampleRate, samplesPerBlock, getTotalNumOutputChannels());
    keyboardState.reset();
    filter.updateFilter(tree, lastSampleRate);
    //Delay
    delay.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
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

void ThinkSynthAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    //get the voice and get the params needed to make the voice
    for (int i = 0; i < mySynth.getNumVoices(); i++)
    {
        //if myVoice sucessfully casts as a SynthVoice*, get the voice and set the params
        if ((myVoice = dynamic_cast<ThinkSynthVoice*>(mySynth.getVoice(i))))
        {
          myVoice->setEnvelopeParams(tree.getRawParameterValue("attack"), tree.getRawParameterValue("decay"), tree.getRawParameterValue("sustain"), tree.getRawParameterValue("release"));
          myVoice->setOscillatorType(tree.getRawParameterValue("wavetype"));
          myVoice->setFilterParams(tree.getRawParameterValue("filterMenu"), tree.getRawParameterValue("filterCutoff"), tree.getRawParameterValue("filterRes"));
        }
    }

    keyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    buffer.clear();
    for(auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      buffer.clear(i, 0, buffer.getNumChannels());

    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    filter.updateFilter(tree, lastSampleRate);

    dsp::AudioBlock<float> block (buffer);

    // Distortion
    if (*tree.getRawParameterValue("distortionTurn")) {
        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            for (int i = 0; i < buffer.getNumSamples(); ++i) {
                float *const samples = buffer.getWritePointer(channel);
                //std::cout << *tree.getRawParameterValue("distortionType") << " " << *tree.getRawParameterValue("distortionAmplitude") << " " << *tree.getRawParameterValue("distortionRatio") << std::endl;
                samples[i] = dist->processSample(samples[i],
                                                 *tree.getRawParameterValue("distortionType"),
                                                 *tree.getRawParameterValue("distortionAmplitude"),
                                                 *tree.getRawParameterValue("distortionRatio"));
            }
        }
    }

    // Reverb
    if (*tree.getRawParameterValue("reverbTurn")) {
        theReverbParameters.roomSize = *tree.getRawParameterValue("roomSize");
        theReverbParameters.dryLevel = *tree.getRawParameterValue("dryLevel");
        theReverbParameters.wetLevel = *tree.getRawParameterValue("wetLevel");
        theReverbParameters.damping = *tree.getRawParameterValue("damping");
        theReverbParameters.width = *tree.getRawParameterValue("width");
        reverb.setParameters(theReverbParameters);
        if (getTotalNumOutputChannels() == 1)
            reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
        else
            reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }

    // Delay
    delay.processBlock(buffer, tree, totalNumOutputChannels);

    // Filter
    filter.process(block);
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
