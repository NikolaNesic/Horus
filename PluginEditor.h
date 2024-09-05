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
class HorusAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HorusAudioProcessorEditor (HorusAudioProcessor&);
    ~HorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HorusAudioProcessor& audioProcessor;
    juce::Slider chorusSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusSliderAttachmentptr;
    juce::Slider rateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachmentptr;
    juce::Slider depthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthSliderAttachmentptr;
    juce::Slider centreSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centreSliderAttachmentptr;
    juce::Slider feedbackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachmentptr;
    juce::Slider mixSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachmentptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HorusAudioProcessorEditor)
};
