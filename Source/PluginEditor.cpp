/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor (MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // WINDOW SIZE
    setSize (800, 450);

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // ADSR Attachments
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);

    oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
}

MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::~MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor()
{
}

//==============================================================================
void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 2);

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

}

void MULTIPLE_OSCILLATOR_VIRTUAL_SYNTHESISERAudioProcessorEditor::setSliderParams(juce::Slider& slider)
{
    //TEMP!
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
