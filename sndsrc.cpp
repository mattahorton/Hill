// name: sndsrc.cpps
// desc: a sound file object / wrapper
//
// author: Music 256a / CS 476a
//   date: fall 2014

#include "sndsrc.h"
#include <sndfile.h>
#include <iostream>
using namespace std;


// read data from file
SAMPLE * readData( const string & filename, int * size, int * srate, int * channels )
{
    // handle
    SNDFILE * sf = NULL;
    // info
    SF_INFO info;
    // because the doc says
    info.format = 0;
    // ...
    SAMPLE * buffer = NULL;
    // zero out
    *size = 0;
    *srate = 0;
    
    // open it
    sf = sf_open( filename.c_str(), SFM_READ, &info );
    // check it
    if( !sf )
    {
        // error message
        cout << "error: cannot open '" << filename << "'" << endl;
        return NULL;
    }
    
    // allocate the whole thing!
    buffer = new SAMPLE[info.frames*info.channels];
    // check it
    if( !buffer )
    {
        // error message
        cout << "error: out of memory... frak" << endl;
        goto done;
    }
    
    // read it
    if( sf_read_float( sf, buffer, info.frames*info.channels ) != info.frames*info.channels )
    {
        // error message
        cout << "error: can't read file..." << endl;
        // free
        delete [] buffer; buffer = NULL;
        goto done;
    }
    
    // set size
    *size = info.frames;
    // set srate
    *srate = info.samplerate;
    // set channels
    *channels = info.channels;
    
done:
    // close sf
    if( sf ) sf_close( sf );
    
    return buffer;
}


SndFileSrc::SndFileSrc()
{
    m_size = 0;
    m_srate = 0;
    m_channels = 0;
    m_buffer = NULL;
    m_playHead = 0;
    m_rate = 1;
    m_gain = 1;
}


SndFileSrc::SndFileSrc( const string & filename )
{
    SndFileSrc();
    read( filename );
}


SndFileSrc::~SndFileSrc()
{
    cleanup();
}


// clean up
void SndFileSrc::cleanup()
{
    if( m_buffer )
    {
        delete [] m_buffer;
        m_buffer = NULL;
    }
    
    m_playHead = 0;
}


// read a sound file
bool SndFileSrc::read( const string & filename )
{
    // clean up
    cleanup();
    // read data
    m_buffer = readData( filename, &m_size, &m_srate, &m_channels );
    // rewind
    rewind();
    
    return (m_buffer != NULL );
}


// rewind
void SndFileSrc::rewind( int position )
{
    m_playHead = position;
}

// size
int SndFileSrc::getSize()
{
    return m_size;
}

// rate
void SndFileSrc::rate( SAMPLE rate )
{
    m_rate = rate;
}

// gain
void SndFileSrc::gain( SAMPLE gain )
{
    m_gain = gain;
}

// synthesize the next buffer (stereo)
bool SndFileSrc::synthesize2( SAMPLE * output, int numFrames )
{
    // debug
    // cerr << "rate: " << m_rate << " playhead: " << m_playHead << endl;
    
    // fill
    for( int i = 0; i < numFrames; i++ )
    {
        // have we reached EOF
        if( m_rate >= 0 && m_playHead >= m_size ) return false;
        if( m_rate < 0 && m_playHead < 0 ) return false;
        
        // copy next frame
        output[i*m_channels] = m_gain * m_buffer[(int)(m_playHead+.5)*m_channels];
        if( m_channels == 2 )
            output[i*m_channels+1] = m_gain * m_buffer[(int)(m_playHead+.5)*m_channels+1];
        
        // increment by 'rate' frame
        m_playHead += m_rate;
    }
    
    return true;
}
