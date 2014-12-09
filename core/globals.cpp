//
//  globals.cpp
//
//
//  Created by Matthew Horton on 10/24/14.
//  Based on bk-globals.h by Ge Wang
//
#include "globals.h"
#include "y-entity.h"


// defaults
#define DEFAULT_FULLSCREEN    FALSE
#define DEFAULT_WINDOW_WIDTH  1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_VERSION       "1.0.0"

// Global simulation
BKSim * Globals::sim = NULL;

// Global mediator
Mediator * Globals::mediator = NULL;

// Global ScoreParser
ScoreParser * Globals::parser = NULL;

// Global Terrain
YEntity * Globals::terrain = NULL;

// Global SndFileSrc
SndFileSrc Globals::sndfile;

// Globals Sukothai effect
Sukothai * Globals::sukothai = NULL;

// Global text object
YText * Globals::text = NULL;

int Globals::currentLine = 0;

bool Globals::started = false;

std::vector<std::string> Globals::lineStrings;

std::vector<float> Globals::lineRadii;

std::vector<float> Globals::lineAngles;

float Globals::mix;

float Globals::maxAvg;

// Shockwave ints
std::array<int,5> Globals::waves = {-1,-1,-1,-1,-1};

// Window size globals
GLsizei Globals::windowWidth = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;

// Audio buffer globals
SAMPLE * Globals::lastAudioBuffer = NULL;
SAMPLE * Globals::lastAudioBufferMono = NULL;
SAMPLE * Globals::audioBufferWindow = NULL;
unsigned int Globals::lastAudioBufferFrames = 0;
unsigned int Globals::lastAudioBufferChannels = 0;
unsigned int Globals::lastAudioBufferBytes = 0;

// Full and blend screen bools
GLboolean Globals::fullscreen = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat Globals::blendRed = 0.0f;
GLenum Globals::fillmode = GL_FILL;

// Default background fill
iSlew3D Globals::bgColor(0, 0, 0, 1.0f); //black
//iSlew3D Globals::bgColor(0.878, 0.894, 0.8, 1.0f); //option 1
//iSlew3D Globals::bgColor(0.027, 0.698, 0.88, 1.0f); //option 2
//iSlew3D Globals::bgColor(0.455, 0.812, 0.682, 1.0f); //option 3
//iSlew3D Globals::bgColor(0.969, 0.678, 0.455, 1.0f); //option 4
std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;
