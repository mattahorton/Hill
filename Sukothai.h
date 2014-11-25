#ifndef _SUKOTHAI_H_
#define _SUKOTHAI_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

#define SAMPLE float
#define FRAMESIZE 512
#define THE_SRATE 44100
#define RANDOMWIN .5

//typedef void (*Callback)(int, int);

class Sukothai
{
  int maxIter;
  int numFrames;
  std::vector<SAMPLE *> iters;
  SAMPLE * input;

public:

  Sukothai(SAMPLE * in, int iterations, int size);
  Sukothai () { Sukothai(NULL, 10, 0); }
  Sukothai(int iterations) { Sukothai(NULL, iterations, 0); }
  ~Sukothai();

public:
  void setInput(SAMPLE * in, int frames);
  SAMPLE * getBuffer(int m);
  int getOutputSize();
  int numBuffs();

private:
  void getMono(SAMPLE * stereo, SAMPLE * mono, int size);
  void setOutputSize(int newS) { m_outputSize = newS; }
  void cleanup();

private:
  int m_outputSize;
};

#endif
