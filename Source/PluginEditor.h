/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor (MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor&);
    ~MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    /*
        ============================================
        UI PARAMS (SLIDERS, BUTTONS, COMBOBOXES, ETC.
        ============================================
    */
    
    //ADSR Sliders/Attachments
    
    //========================

    void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::setSliderParams(juce::Slider& slider);
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::ComboBox oscSelector;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    //========================
    
    //OSC Selector
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;

    //FUTURE ATTACHMENTS
    //------------------
    MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor)
};
