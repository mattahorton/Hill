//-----------------------------------------------------------------------------
// name: audio.h
// desc: audio stuff
//
// author: Matt Horton (mattah@ccrma.stanford.edu), after Ge Wang
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vector>


// init audio
bool audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool audio_start();
// get next power of 2
unsigned long next_power_2(unsigned long n);


#endif
