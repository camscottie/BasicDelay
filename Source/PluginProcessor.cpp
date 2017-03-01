/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BasicDelayAudioProcessor::BasicDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	// L A B  5   C O D E 

	/* The code below assigns default values to the variables defined in PluginProcessor.h
	   This is done to encourage good practice of assigning variables for program startup.
	*/

		// Some Feedback
		feedback = 0.5;

		//Delay of 0.2 seconds
		delayTime = 0.25;

		//Start reading from the start of the circular buffer
		readIndex = 0;

		//Set the writeIndex ahead of the read index, using the delayTime variable
		writeIndex = delayTime;

		//Initial delay buffer size
		delayBufferLength = 0;


}

BasicDelayAudioProcessor::~BasicDelayAudioProcessor()
{
}

//==============================================================================
const String BasicDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double BasicDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicDelayAudioProcessor::getProgramName (int index)
{
    return String();
}

void BasicDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BasicDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
	/* This function is called before the playback starts,
	   The Sample rate is also set */

{
	
	// L A B  5   C O D E 

		// Maximum delay of 1 second
		delayBufferLength = (int)(sampleRate);

			/* The (int)(Variable) syntax is known as casting
			   Casting is converting a variables type to another
			   In the above case, a double is changed to an int.

			   Another method of casting can be written as:		(Shown to me by Ollie)
			   int x;
			   x = static_cast<int>(sampleRate);   */   

		
		//Set the buffer to 1 channel of the size of delayBufferLength using setSize
		delayBuffer.setSize(1, delayBufferLength); 

		//Set all the samples in the buffer to zero
		delayBuffer.clear();

		// Calculate the position of the read index, by looking at the write index position
		// (This is the delay sample buffer size)
		readIndex = (int)(writeIndex - (delayTime * delayBufferLength) + delayBufferLength) % delayBufferLength;

			/* Variables used in the above equation:
					writeIndex = delayTime = 0.25
					delayTime = 0.25
					delayBufferLength = Sample Rate

			   The above equation is stating that in this function (prepareToPlay):
					(int) - Cast the result as an int
					(0.25 - (0.25 * sampleRate) + sampleRate)
					remainder sampleRate

			*/

						
			
}

void BasicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicDelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

		// L A B  5   C O D E

			// S T A R T U P   C O D E

				// 1. channelData = an array which contains audio for one channel
				// 2. channelData's length is defined by the numSamples variable.
				float *channelData = buffer.getWritePointer(0);

				// 1. delayData is a circular buffer
				// 2. delayData is for implementing the delay
				float *delayData = delayBuffer.getWritePointer(0);

				// 1. Set a wet mix level
				float wetMix = 0.5;

			// M A I N   P R O C E S S O R  L O O P
			for (int i = 0; i < buffer.getNumSamples(); ++i)
			{
				// 1. Calculate the next output sample
				// 2. This is calculated as: 
				// 3. (Current input sample + delayed version)
				float outputSample = (channelData[i] + (wetMix * delayData[readIndex]));

				// 1. Write the current input into the delay buffer
				// 2. AND write the delayed sample into the delay buffer
				delayData[writeIndex] = channelData[i] + (wetMix * delayData[readIndex] * feedback)

				// 1. Increment the readIndex
				// 2. Check to see if readIndex greater than the buffer length
				// 3. If readIndex > the buffer length
				// 4. Change readIndex back to zero
				if (++readIndex >= delayBufferLength)
					readIndex = 0;

				// 1. Do the same with the writeIndex
				if (++writeIndex >= delayBufferLength)
					writeIndex = 0;

				// 1. Assign output sample computed above to the output buffer
				channelData[i] = outputSample;
				
			}
}

//==============================================================================
bool BasicDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BasicDelayAudioProcessor::createEditor()
{
    return new BasicDelayAudioProcessorEditor (*this);
}

//==============================================================================
void BasicDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicDelayAudioProcessor();
}
