/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class BasicDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    BasicDelayAudioProcessor();
    ~BasicDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

		// M Y  P U B L I C   V A R I A B L E S 
		
			//enumerator for the parameter functions
			enum Parameters
			{
				kDelayTimeParam = 0,   // 0
				kFeedbackParam,		   // 1
				kNumParameters		   // 2
			};
			
			// Determines how loud the sound feeds back
			float feedback;
		
			// Determines the 
			float delayTime;

			//
			int getNumParamenters();

			//
			float getParameter(int index);

			//
			void setParameter(int index, float newValue);

			//
			const String getParameterName(int index);

			//
			const String getParameterText(int index);

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicDelayAudioProcessor)

		// M Y  P R I V A T E   V A R I A B L E S 
			
			// The type 'AudioSampleBuffer' provides the functionality of a biasci audio buffer
			// This is specifically used for a circular buffer here
			AudioSampleBuffer delayBuffer;
				/* DEFINITION - AudioSampleBuffer
				A multi-channel buffer of 32-bit floating point audio samples.
				This typedef is here for backwards compatibility with the older AudioSampleBuffer class, 
				which was fixed for 32-bit data, but is otherwise the same as the new templated AudioBuffer class.
				*/
			
			// Determines the size of the Delay Buffer
			int delayBufferLength;

			// Determines the position in the buffer to read a sample from
			int readIndex;

			// Determines the position in the buffer to write a sample to
			int writeIndex;
		
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
