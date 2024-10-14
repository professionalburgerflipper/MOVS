/*
  ==============================================================================
    SynthVoice.cpp
    Created: 5 Oct 2024 4:55:26pm
    ----
    This file handles the voice of MOVS.
    Props to TheAudioProgrammer for his JUCE Synthesiser tutorial <3
  ==============================================================================
*/

#include "SynthVoice.h"

//Bool for asking if MOVS can play sound
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};

//Sets note based off of midi input (including keyboard, and DAW piano roll)
//also turns on ADSR
void SynthVoice::startNote(int midNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midNoteNumber));
    adsr.noteOn();
};

//Handle stopNote functions, such as stopping ADSR, and giving tailoff
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
    {
        clearCurrentNote();
    }
};

//Unused for now.
//Used for eventual MIDI support
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

};

//Unused for now.
void SynthVoice::pitchWheelMoved(int pitchWheelValue)
{

};

//Set sample rate, maximumBlockSize, numChannels and gain, preparing for the synth to play
//Only used if isPrepared == false.
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepare(spec);
    gain.prepare(spec);

    gain.setGainLinear(0.5);

    isPrepared = true;
};

//Update the ADSR values equal to the sliders present in the UI, this accounts for attack, decay, sustain, and release
//Example line: UpdateADSR(1, 0, 1, 0.5) [random values]
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParam.attack = attack;
    adsrParam.decay = decay;
    adsrParam.sustain = sustain;
    adsrParam.decay = release;

    adsr.setParameters(adsrParam);
};

//Render the next block of sound to play, and applies ADSR and other filters (not implemented). The note played is determined in startNote()
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSample)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    // render oscillator
    // this uses the waves set SynthVoice.h

    // this function is called by SynthVoice.h
    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // apply ADSR to oscillator 
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSample);
};