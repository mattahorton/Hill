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
  // Preferred constructor takes in signal, number of Sukothai iterations
  // and size of the signal in frames. The number of samples in the signal
  // is equal to size * the number of channels.
  Sukothai(SAMPLE * in, int iterations, int size);

  // Courtesy constructors
  Sukothai () { Sukothai(NULL, 10, 0); }
  Sukothai(int iterations) { Sukothai(NULL, iterations, 0); }
  ~Sukothai();

public:
  void setInput(SAMPLE * in, int frames);
  SAMPLE * getBuffer(int m);
  // Returns the size of the audio buffers in iters in terms of frames
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
