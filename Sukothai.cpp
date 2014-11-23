#include "Sukothai.h"
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

Sukothai::Sukothai(SAMPLE * in, int iterations, int size) {
  maxIter = iterations;
  if(in != NULL) setInput(in);
  numFrames = size;
  m_outputSize = 0;
}

void Sukothai::setInput(SAMPLE * in) {
  // per-iteration additional time in terms of seconds
  float extraTime = RANDOMWIN*2;

  // new buffer size in terms of samples
  int newSize = ((int)(maxIter*extraTime*2*THE_SRATE)+numFrames*FRAMESIZE*2);
  cerr << newSize << " newSize" << endl;

  // half of the additional time in terms of samples
  int halfExtra;

  SAMPLE * current_buffer = new SAMPLE[newSize];
  SAMPLE * mono;
  int startl = 0, startr = 0;
  int delay;
  int currentMonoSize = 0;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution(-RANDOMWIN,RANDOMWIN);

  for(int i = 0; i < maxIter; i++) {
    delay = (int)(distribution(generator)*THE_SRATE);

    //Zero out current buffer
    for (int j = 0; j < newSize; j++){
        current_buffer[j] = 0;
    }

    // create mono signal out of two channels
    if (i == 0) {
      currentMonoSize = numFrames*FRAMESIZE;
      mono = getMono(in,currentMonoSize*2);
    } else {
      currentMonoSize = newSize/2;
      mono = getMono(iters.at(i-1),currentMonoSize*2);
    }

    halfExtra = (newSize - currentMonoSize*2)/2;
    cerr << halfExtra;

    //copy in new vals
    startl = halfExtra + delay;
    startr = halfExtra - delay;


    //cerr << currentMonoSize << " mono size" << endl;
    // cerr << newSize << " current_buffer size" << endl;
    // cerr << halfExtra << " halfExtra" << endl;
    // cerr << startr << " startr" << endl;
    // cerr << startl << " startl" << endl;

    SAMPLE x = 0.0;

    cerr << current_buffer[0] << endl;
    cerr << startl << " " << current_buffer[startl] << endl;
    cerr << startr << " " << current_buffer[startr] << endl;

    for (int k = 0; k < currentMonoSize; k++) {
      //cerr << k+startl << " k" << endl;

      // if(i ==0) {
      //   cerr << mono[k] << " mono[k]" << endl;
      //   cerr << current_buffer[k+startl] << " value - index " << k+startl << endl;
      //   cerr << current_buffer[k+startr+1] << " value - index " << k+startr+1 << endl;
      //   x = mono[currentMonoSize];
      // }
      if(k+startl > 0) current_buffer[k+startl] = current_buffer[k+startl] + mono[k];
      if(k+startr+1 > 0) current_buffer[k+startr+1] = current_buffer[k+startr] + mono[k];
      // if(i ==0) {
      //   cerr << mono[k] << " mono[k]" << endl;
      //   cerr << current_buffer[k+startl] << " value - index " << k+startl << endl;
      //   cerr << current_buffer[k+startr+1] << " value - index " << k+startr+1 << endl;
      //   x = mono[currentMonoSize];
      // }

    }

    cerr << current_buffer[0] << endl;
    cerr << startl << " " << current_buffer[startl] << endl;
    cerr << startr << " " << current_buffer[startr] << endl;


    iters.push_back(current_buffer);
    //cerr << startl << endl;
    //cerr << startr << endl;
  }


  m_outputSize = newSize/2;
  cerr << m_outputSize << " outputSize" << endl;
}

SAMPLE * Sukothai::getBuffer(int m) {
  return iters.at(m);
}

int Sukothai::getOutputSize() {
  return m_outputSize;
}

SAMPLE * Sukothai::getMono(SAMPLE * stereo, int size){
  SAMPLE * mono = new SAMPLE[size/2];

  for (int j = 0; j < size/2; j++) {
    mono[j] = (stereo[j*2] + stereo[j*2+1])/sqrt(2);
  }

  return mono;
}
