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




XMutex g_mutex;
SAMPLE * input;
int g_sample = 0;

//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    int channels = XAudioIO::numChannels();

    // zero out for output
    memset( buffer, 0, sizeof(SAMPLE)*numFrames*XAudioIO::numChannels() );

    if(Globals::started) {
      if((Globals::sukothai->getOutputSize() != 0) && (Globals::sukothai->numBuffs() > 0)) {
        input = Globals::sukothai->getBuffer(9);
        Globals::sndfile.synthesize2( buffer, input, numFrames, Globals::sukothai->getOutputSize() );
      }

      // Globals::sndfile.synthesize2( buffer, numFrames);
      g_sample = Globals::sndfile.playhead();
      for (int i = 0; i < numFrames; i++) {
        Globals::mediator->updateCount(g_sample);
        g_sample++;
      }
    }
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
