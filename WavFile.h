//
//  WavFile.h
//  Poppytastic
//
//  Created by Chet Gnegy on 5/1/14.
//  Copyright (c) 2014 Chet Gnegy. All rights reserved.
//

#ifndef __WAVFILE_H__
#define __WAVFILE_H__

struct WavFile {
    bool loop;
    bool play;
    // How far we are on a fade out;
    long fade;
    long fade_length;
    // The number of channels
    int channels;
    // The length of a single channel
    int length;
    // Position in the track along one channel
    int pos;
    // The audio data
    float *track;
    // Volume
    float gain;
    int just_started;
};

#endif
