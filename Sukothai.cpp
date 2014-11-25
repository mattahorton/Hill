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
  //cerr << numFrames << endl;
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
  float extraTime = RANDOMWIN*2;

  // new buffer size in terms of samples
  //cerr << frames << endl;
  int newSize = ((int)(maxIter*extraTime*2*THE_SRATE)+frames*FRAMESIZE*2);
  //cerr << newSize << " newSize" << endl;

  // half of the additional time in terms of samples
  int halfExtra;

  SAMPLE * current_buffer = new SAMPLE[newSize];
  SAMPLE * mono;
  int startl = 0, startr = 0;
  int delay;
  int currentMonoSize = 0;

  // seed new normal distribution generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution(-RANDOMWIN,RANDOMWIN);
  cerr << "hey" << endl;
  for(int i = 0; i < maxIter; i++) {
    // Get new delay
    delay = (int)(distribution(generator)*THE_SRATE);

    //Zero out current buffer
    cerr << "hey" << endl;
    memset(current_buffer,0,newSize);
    cerr << "before mono" << endl;

    // create mono signal out of two channels
    if (i == 0) {
      currentMonoSize = frames*FRAMESIZE;
      mono = new SAMPLE[currentMonoSize];
      getMono(in, mono, currentMonoSize);
    } else {
      currentMonoSize = newSize/2;
      mono = new SAMPLE[currentMonoSize];
      getMono(Sukothai::iters.at(i-1), mono, currentMonoSize);
    }
    cerr << "after mono" << endl;
    halfExtra = (newSize - currentMonoSize*2)/2;

    //copy in new vals
    startl = halfExtra + delay;
    startr = halfExtra - delay;

    SAMPLE x = 0.0;
    cerr << "before processing " << endl;
    for (int k = 0; k < currentMonoSize; k++) {
      if(k+startl > 0) current_buffer[k+startl] = current_buffer[k+startl] + mono[k];
      if(k+startr+1 > 0) current_buffer[k+startr+1] = current_buffer[k+startr] + mono[k];
    }

    Sukothai::iters.push_back(current_buffer);

    delete [] mono;
  }


  setOutputSize(newSize/2);
  //cerr << m_outputSize << " outputSize" << endl;
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
    mono[j] = (stereo[j*2] + stereo[j*2+1])/sqrt(2);
  }
}
