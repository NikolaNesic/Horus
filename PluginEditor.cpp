/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HorusAudioProcessorEditor::HorusAudioProcessorEditor (HorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 600);
    addAndMakeVisible(chorusSlider);
    chorusSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    chorusSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "input", chorusSlider));
    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "rate", rateSlider));
    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "depth", depthSlider));
    addAndMakeVisible(centreSlider);
    centreSlider.setSliderStyle(juce::Slider::Rotary);
    centreSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "centre", centreSlider));
    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::Rotary);
    feedbackSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "feedback", feedbackSlider));
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "mix", mixSlider));
}

HorusAudioProcessorEditor::~HorusAudioProcessorEditor()
{
}

//==============================================================================
void HorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::rebeccapurple);

    g.setColour(juce::Colours::white);
    g.setFont(14);
    g.drawFittedText("Gain (dB)", chorusSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Rate (Hz)", rateSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Depth", depthSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Centre Delay (ms)", centreSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Feedback", feedbackSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Mix", mixSlider.getBounds(), juce::Justification::centredBottom, 1);

    //juce::Line<float> line(juce::Point<float>(0, 100), juce::Point<float>(200, 100));
    //g.drawLine(line, 1.0f);
    juce::Line<float> line(juce::Point<float>(0, 100), juce::Point<float>(200, 100));
    g.drawLine(line, 1.0f);
    juce::Line<float> line1(juce::Point<float>(0, 200), juce::Point<float>(200, 200));
    g.drawLine(line1, 1.0f);
    juce::Line<float> line2(juce::Point<float>(0, 300), juce::Point<float>(200, 300));
    g.drawLine(line2, 1.0f);
    juce::Line<float> line3(juce::Point<float>(0, 400), juce::Point<float>(200, 400));
    g.drawLine(line3, 1.0f);
    juce::Line<float> line4(juce::Point<float>(0, 500), juce::Point<float>(200, 500));
    g.drawLine(line4, 1.0f);
    juce::Line<float> line5(juce::Point<float>(0, 600), juce::Point<float>(200, 600));
    g.drawLine(line5, 1.0f);
}

void HorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    //chorusSlider.setBounds(bounds.removeFromTop(bounds.getHeight()));
    chorusSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.166));
    rateSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.2));
    depthSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.25));
    centreSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.333));
    feedbackSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    mixSlider.setBounds(bounds.removeFromTop(bounds.getHeight()));
}
