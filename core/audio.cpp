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
int g_currentTick = 0;


//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    // int channels = XAudioIO::numChannels();

    // zero out for output
    memset( buffer, 0, sizeof(SAMPLE)*numFrames*XAudioIO::numChannels() );
    //memset( buffer, 0, sizeof(SAMPLE)*numFrames*channels );
    // fill_n(buffer, sizeof(SAMPLE)*numFrames*channels, 0);
    // cerr << sizeof(buffer) << endl;

    //memcpy(buffer, Globals::wav->track, sizeof(SAMPLE)*numFrames*channels);
    //copy(begin(Globals::wav->track), end(Globals::wav->track), begin(buffer));

    // if((g_currentTick < Globals::wav->length) && ((Globals::wav->length - g_currentTick) > numFrames)) {
    //   for (int i = 0; i < numFrames*channels; i += channels){
    //     buffer[i] = (SAMPLE)Globals::wav->track[g_currentTick+i];
    //     buffer[i+1] = (SAMPLE)Globals::wav->track[g_currentTick+i+1];
    //     // cerr << i << " " << i+1 << endl;
    //     // cerr << buffer[i] << " " << buffer[i+1] << endl;
    //     // cerr << buffer[i+XAudioIO::numChannels()];
    //   }
    //
    //   // cerr << sizeof(buffer)/sizeof(SAMPLE) << endl;
    //   // cerr << "here" << endl;
    // } else if ((Globals::wav->length - g_currentTick) < numFrames) {
    //   for (int i = 0; i < numFrames*channels; i += channels){
    //     if (i < numFrames - g_currentTick) {
    //       buffer[i] = (SAMPLE)Globals::wav->track[g_currentTick+i];
    //       buffer[i+1] = (SAMPLE)Globals::wav->track[g_currentTick+i+1];
    //     } else {
    //       buffer[i] = 0;
    //       buffer[i+1] = 0;
    //     }
    //     buffer[i+XAudioIO::numChannels()] = Globals::wav->track[i];
    //     // cerr << buffer[i+XAudioIO::numChannels()];
    //   }
    // }
    //
    // g_currentTick += numFrames;

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
