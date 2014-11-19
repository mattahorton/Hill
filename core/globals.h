//
//  globals.h
//
//
//  Created by Matthew Horton on 10/24/14.
//  Based on bk-globals.h by Ge Wang
//

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"
#include "y-entity.h"
#include "ScoreParser.h"
#include "WavFile.h"

// c++
#include <string>
#include <map>
#include <vector>
#include <array>
#include <utility>
#include <queue>

// defines
#define THE_SRATE        44100
#define FRAMESIZE    512
#define NUMCHANNELS  2
#define MAX_TEXTURES 32
#define TAN 0.878, 0.894, 0.8
#define BLUE 0.027, 0.698, 0.88
#define GREEN 0.455, 0.812, 0.682
#define ORANGE 0.969, 0.678, 0.455
#define LINEON 0.953f, 0.525f, 0.188f
#define LINEOFF 0.655, 0.859, 0.859



// forward references
class BKSim;
class Mediator;
class YEntity;
class YText;


//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // top level root simulation
    static BKSim * sim;

    // global mediator
    static Mediator * mediator;

    static ScoreParser * parser;

    static WavFile * wav;

    static YEntity * terrain;

    static YText * text;

    // Shockwave globals
    static std::array<int,5> waves;

    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;

    // last audio buffer
    static SAMPLE * lastAudioBuffer;
    static SAMPLE * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;
    static unsigned int lastAudioBufferBytes;

    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;

    // graphics fullscreen
    static GLboolean fullscreen;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;

};

#endif
