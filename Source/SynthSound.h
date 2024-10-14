/*
  ==============================================================================

    SynthSound.h
    Created: 5 Oct 2024 4:56:10pm
    ----
    This file really just handles bools for now, will be used more later in development.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
    public:
        bool appliesToNote(int midiNoteNunber) override { return true; }
        bool appliesToChannel(int midiChannel) override { return true; }

};