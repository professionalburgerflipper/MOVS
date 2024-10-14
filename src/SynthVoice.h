/*
  ==============================================================================

    SynthVoice.h
    Created: 5 Oct 2024 4:55:26pm
    ----
    HEADER file for SynthVoice.cpp, this file sets all the functions used in SynthVoice, and presets the oscillator values
    for the wave, gain, ADSR, and buffer.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    /*
        ------------------------------------
        Set functions used in SynthVoice.cpp
        ------------------------------------
    */

    //Bool for asking if MOVS can play sound
    bool canPlaySound(juce::SynthesiserSound * sound) override;

    //Sets note based off of midi input (including keyboard, and DAW piano roll)
    //also turns on ADSR
    void startNote(int midNoteNumber, float velocity, juce::SynthesiserSound * sound, int currentPitchWheelPosition) override;

    //Handle stopNote functions, such as stopping ADSR, and giving tailoff
    void stopNote(float velocity, bool allowTailOff) override;

    //Unused for now
    //Used for eventual MIDI support
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    //Unused for now
    void pitchWheelMoved(int pitchWheelValue) override;

    //Set sample rate, maximumBlockSize, numChannels and gain, preparing for the synth to play
    //Only used if isPrepared == false.
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    //Update the ADSR values equal to the sliders present in the UI, this accounts for attack, decay, sustain, and release
    //Example line: UpdateADSR(1, 0, 1, 0.5) [random values]
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

    //Render the next block of sound to play, and applies ADSR and other filters (not implemented). The note played is determined in startNote()
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSample) override;
    
private:

    /*
    =====================
    |   WAVE PRESETS    |
    =====================
     
        SINE WAVE
        juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

        SAW WAVE
        juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; } };

        SQUARE WAVE

        NOISE

    =====================
    */

    //Set the rule for the oscillator wave to follow.
    //This current setup is temporary and only allows for one wave type to be hardcoded in, in the future a combobox will be
    //able change it. This will most likely be moved to SynthVoice.cpp inside it's own function.
    juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; } };

    //Temporary way of setting gain, much like the line before this, this will be able to be ajusted by a slider in the future and this line
    //will most likely be moved to it's own function in SynthVoice.cpp
    juce::dsp::Gain<float> gain;

    //Preset ADSR, these will be changed in runtime by the updateADSR function in SynthVoice.cpp
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParam;

    //This line of code is most likely depricated, but i'm scared to remove it .-.
    juce::AudioBuffer<float> synthBuffer;
    
    //Set isPrepared to false on startup to ensure the isPrepared function in SynthVoice.cpp goes through.
    bool isPrepared{ false };
};
