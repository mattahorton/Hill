// name: sndsrc.h
// desc: a sound file object / wrapper
//
// author: Music 256a / CS 476a
//   date: fall 2014

#ifndef __SNDSRC_H__
#define __SNDSRC_H__

#include <string>
using namespace std;

// our datatype
#define SAMPLE float


// class definition
class SndFileSrc
{
public:
    // default constructor
    SndFileSrc();
    // overloaded constructor
    SndFileSrc( const string & filename );
    // destructor
    ~SndFileSrc();

public:
    // clean up
    void cleanup();
    // read a sound file
    bool read( const string & filename );
    // rewind to pos
    void rewind( int position = 0 );
    // get number of frames in file
    int getSize();
    // get playhead
    const int playhead() { return m_playHead; }
    // rate
    void rate( SAMPLE rate );
    // gain (volume)
    void gain( SAMPLE gain );
    // synthesize the next buffer (stereo)
    bool synthesize2( SAMPLE * output, int numFrames );
    // synthesize the next buffer (stereo) from an input buffer
    bool synthesize2( SAMPLE * output, SAMPLE * input, int numFrames, int size );
    // return wav buffer
    SAMPLE * getAudio();

protected: // data
    int m_size; // length of file (in frames)
    int m_srate; // sample rate of file
    int m_channels; // number of channers in file
    SAMPLE * m_buffer; // audio buffer
    SAMPLE m_playHead; // play head
    SAMPLE m_rate; // play rate
    SAMPLE m_gain; // volume
};




#endif
