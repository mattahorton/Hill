//-----------------------------------------------------------------------------
// name: audio.cpp
// desc: audio stuff for KeySeq
//
// author: Matt Horton (mattah@ccrma.stanford.edu), after Ge Wang
//   date: 2014
//-----------------------------------------------------------------------------
#include "audio.h"
#include "globals.h"
#include "Mediator.h"
#include "bk-sim.h"
#include "y-fft.h"
#include "x-thread.h"
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

int getMaxIdx(SAMPLE * buff, int length);


XMutex g_mutex;
SAMPLE * input;
int g_sample = 0;
int buffCount = 0;
int avgSize = 200;
float * maxs = new float[avgSize];

//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    Globals::ter->resetMinY();

    int channels = XAudioIO::numChannels();
    int mixBuff = 0;
    SAMPLE * fftBuff = new SAMPLE[numFrames*XAudioIO::numChannels()];

    // zero out for output
    memset( buffer, 0, sizeof(SAMPLE)*numFrames*XAudioIO::numChannels() );

    mixBuff = (int) Globals::mix;

    if(Globals::started) {

      // Play next buffer
      if((Globals::sukothai->getOutputSize() != 0) && (Globals::sukothai->numBuffs() > 0)) {
        input = Globals::sukothai->getBuffer(mixBuff - 1);
        Globals::sndfile.synthesize2( buffer, input, numFrames, Globals::sukothai->getOutputSize() );

        // Get max idx
        maxs[buffCount%avgSize] = getMaxIdx(buffer,numFrames);
        float sum = 0.0f;
        for (int i = 0; i < avgSize; i++) {
          sum += maxs[i];
        }
        Globals::maxAvg = sum/(float)avgSize;
      }

      // Tell the Mediator where we are
      g_sample = Globals::sndfile.playhead();
      for (int i = 0; i < numFrames; i++) {
        Globals::mediator->updateCount(g_sample);
        g_sample++;
      }
    }
    buffCount++;
}

//-----------------------------------------------------------------------------
// name: getMaxIdx()
// desc: get index of max value in a buffer
//-----------------------------------------------------------------------------
int getMaxIdx(SAMPLE * buff, int length){
  int max = 0;
  for (int i = 1; i < length; i += 2) {
    if(buff[i] > buff[max]) max = i;
  }
  return max;
}

//-----------------------------------------------------------------------------
// name: next_power_2()
// desc: ...
// thanks: to Niklas Werner / music-dsp
//-----------------------------------------------------------------------------
unsigned long next_power_2( unsigned long n )
{
  unsigned long nn = n;
  for( ; n &= n-1; nn = n );
    return nn * 2;
  }

//-----------------------------------------------------------------------------
// name: audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    // initialize
    if( !XAudioIO::init( 0, 0, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }


    // allocate
    Globals::lastAudioBuffer = new SAMPLE[frameSize*channels];
    // allocate mono buffer
    Globals::lastAudioBufferMono = new SAMPLE[frameSize];
    // allocate window buffer
    Globals::audioBufferWindow = new SAMPLE[frameSize];
    // set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames = frameSize;
    // set num channels
    Globals::lastAudioBufferChannels = channels;

    // compute the window
    hanning( Globals::audioBufferWindow, frameSize );

    return true;
}




//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
bool audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }

    return true;
}
