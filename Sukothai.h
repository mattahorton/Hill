#ifndef _SUKOTHAI_H_
#define _SUKOTHAI_H_

#include <stdlib.h>
#include <vector>

#define SAMPLE float
#define FRAMESIZE 512
#define THE_SRATE 44100
#define RANDOMWIN .5

//typedef void (*Callback)(int, int);

class Sukothai
{
  int maxIter;
  int numFrames;
  int m_outputSize;
  std::vector<SAMPLE *> iters;
  SAMPLE * input;

public:

  Sukothai(SAMPLE * in, int iterations, int size);
  Sukothai () { Sukothai(NULL, 10, 0); }
  Sukothai(int iterations) { Sukothai(NULL, iterations, 0); }

public:
  void setInput(SAMPLE * in);
  SAMPLE * getBuffer(int m);
  int getOutputSize();

private:
  SAMPLE * getMono(SAMPLE * stereo, int size);
};

#endif
