#include "Sukothai.h"
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

Sukothai::Sukothai(SAMPLE * in, int iterations, int size) {
  //cerr << size << endl;
  maxIter = iterations;
  numFrames = size;
  if(in != NULL) setInput(in, numFrames);
}

Sukothai::~Sukothai()
{
  cleanup();
}


// clean up
void Sukothai::cleanup()
{
  if( input )
  {
    delete [] input;
    input = NULL;
  }

  for (auto it = iters.begin(); it != iters.end(); ++it){
    delete [] *it;
  }
  iters.clear();
}

void Sukothai::setInput(SAMPLE * in, int frames) {
  // per-iteration additional time in terms of seconds
  float extraTime = RANDOMWIN;

  // new buffer size in terms of samples
  int newSize = ((int)(maxIter*extraTime*2*THE_SRATE)+frames*2);

  // half of the additional time in terms of samples
  int halfExtra;

  SAMPLE * mono;
  int startl = 0, startr = 0;
  int delay;
  int currentMonoSize = 0;


  //seed new normal distribution generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution(-RANDOMWIN,RANDOMWIN);

  for(int i = 0; i < maxIter; i++) {
    // Get new delay
    delay = (int)(distribution(generator)*THE_SRATE*2);

    //Zero out current buffer
    SAMPLE * current_buffer = new SAMPLE[newSize];
    memset(current_buffer,0,newSize);

    // create mono signal out of two channels
    if (i == 0) {
      currentMonoSize = frames;
      mono = new SAMPLE[currentMonoSize];
      getMono(in, mono, currentMonoSize);
    } else {
      currentMonoSize = newSize/2;
      mono = new SAMPLE[currentMonoSize];
      getMono(Sukothai::iters.at(i-1), mono, currentMonoSize);
    }

    //get half of the extra time in samples
    halfExtra = (newSize - currentMonoSize*2)/2;

    //create start index offset
    startl = halfExtra + delay;
    startr = halfExtra - delay;

    //copy in new vals
    for (int k = 0; k < currentMonoSize; k++) {
      if((k*2+startl > 0) && (k*2+startl < newSize)) current_buffer[k*2+startl] = mono[k];
      if(k*2+startr+1 > 0 && (k*2+startr+1 < newSize)) current_buffer[k*2+startr+1] = mono[k];
    }

    Sukothai::iters.push_back(current_buffer);

    delete [] mono;
  }


  setOutputSize(newSize/2);
}

SAMPLE * Sukothai::getBuffer(int m) {
  return iters.at(m);
}

int Sukothai::numBuffs() {
  return iters.size();
}

int Sukothai::getOutputSize() {
  return m_outputSize;
}

void Sukothai::getMono(SAMPLE * stereo, SAMPLE * mono, int monoSize){
  for (int j = 0; j < monoSize; j++) {
    mono[j] = .7071*(stereo[j*2] + stereo[j*2+1]);
  }
}
