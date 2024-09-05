/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HorusAudioProcessor::HorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("input", this);
    treeState.addParameterListener("output", this);

    treeState.addParameterListener("rate", this);
    treeState.addParameterListener("depth", this);
    treeState.addParameterListener("centre", this);
    treeState.addParameterListener("feedback", this);
    treeState.addParameterListener("mix", this);
}

HorusAudioProcessor::~HorusAudioProcessor()
{
    treeState.removeParameterListener("input", this);
    treeState.removeParameterListener("output", this);

    treeState.removeParameterListener("rate", this);
    treeState.removeParameterListener("depth", this);
    treeState.removeParameterListener("centre", this);
    treeState.removeParameterListener("feedback", this);
    treeState.removeParameterListener("mix", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout HorusAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto pInput = std::make_unique<juce::AudioParameterFloat>("input", "Gain", -24.0f, 24.0f, 0.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>("output", "Level", -24.0f, 24.0f, 0.0f);

    auto pRate = std::make_unique<juce::AudioParameterFloat>("rate", "Rate", 1.0f, 99.0f, 1.0f);
    auto pDepth = std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, 1.0f, 0.0f);
    auto pCentre = std::make_unique<juce::AudioParameterFloat>("centre", "Centre", 1.0f, 99.0f, 1.0f);
    auto pFeedback = std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", -1.0f, 1.0f, 0.0f);
    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.0f);
   
    params.push_back(std::move(pInput));
    params.push_back(std::move(pOutput));
    params.push_back(std::move(pRate));
    params.push_back(std::move(pDepth));
    params.push_back(std::move(pCentre));
    params.push_back(std::move(pFeedback));
    params.push_back(std::move(pMix));

    
    return { params.begin(), params.end() };
}

void HorusAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}

void HorusAudioProcessor::updateParameters()
{
    inputModule.setGainDecibels(treeState.getRawParameterValue("input")->load());
    outputModule.setGainDecibels(treeState.getRawParameterValue("output")->load());

    chorusModule.setRate(treeState.getRawParameterValue("rate")->load());
    chorusModule.setDepth(treeState.getRawParameterValue("depth")->load());
    chorusModule.setCentreDelay(treeState.getRawParameterValue("centre")->load());
    chorusModule.setFeedback(treeState.getRawParameterValue("feedback")->load());
    chorusModule.setMix(treeState.getRawParameterValue("mix")->load());
}

//==============================================================================
const juce::String HorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void HorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    inputModule.prepare(spec);
    inputModule.setRampDurationSeconds(0.02);

    outputModule.prepare(spec);
    outputModule.setRampDurationSeconds(0.02);

    chorusModule.prepare(spec); // ovde ubaciti ostale parametre ovako
    limiterModule.prepare(spec);
    limiterModule.setThreshold(0.99);
    limiterModule.setRelease(1.0f);
    updateParameters();
}

void HorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block{ buffer };

    inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    chorusModule.process(juce::dsp::ProcessContextReplacing<float>(block)); // i ovde
    limiterModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool HorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HorusAudioProcessor::createEditor()
{
    return new HorusAudioProcessorEditor (*this);
}

//==============================================================================
void HorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
}

void HorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
    {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HorusAudioProcessor();
}
