//PLUGIN PROCESSING

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::~MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor()
{

}

//==============================================================================
const juce::String MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::setCurrentProgram (int index)
{
    //Unused for now
}

const juce::String MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getProgramName (int index)
{
    return {};
}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    //Unused for now
}

//==============================================================================
void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    //Set Input/Output Channels
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //Clear buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            //ADSR Updating, based on the UI sliders from AudioProcesssorValueTreeState
            //Set ADSR through ParamValues
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            //Update ADSR in SynthVoice.cpp
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
    //Call synth renderer in SynthVoice.cpp
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::createEditor()
{
    return new MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor (*this);
}

//==============================================================================
void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor();
}

// Value Tree for setting Params to be able to change in MOVS UI

juce::AudioProcessorValueTreeState::ParameterLayout MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor::createParams()
{
    // combobox : switch oscillators
    // seperate ADSR slider params

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //Add parameters for wave combobox
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray {"Sine", "Saw", "Square"}, 0));

    //Future params for other values, such as:
    //GAIN
    //PITCH
    //OTHER OSILLATORS
    //RENDER MODE

    //Add parameters for ADSR to the params array
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 3.0f, }, 0.4f));

    return { params.begin(), params.end() };
}