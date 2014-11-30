#include "Mediator.h"
#include "globals.h"
#include <iostream>
using namespace std;

Mediator::Mediator( )
{
    currentSamp = 0;
    std::vector<int> periods;
    std::vector<Callback> callbacks;
    std::vector<int> fireTimes;
    std::vector<Callback> singleCallbacks;
}


void Mediator::updateCount(int sampleCount){
  currentSamp = sampleCount;
  int per, ft;
  Callback cb;

  for (int i = 0; i < periods.size(); i++) {
    per = periods.at(i);
    cb = callbacks.at(i);

    if ((sampleCount % per) == 0) {
    }

  }

  for (int i = 0; i < fireTimes.size(); i++) {
    ft = fireTimes.at(i);
    cb = singleCallbacks.at(i);

    if (sampleCount == ft) {
      cb();
    }

  }
}

void Mediator::registerCallback(int period, Callback cb) {
  periods.push_back(period);
  callbacks.push_back(cb);
}

void Mediator::registerSingleCallback(int fireTime, Callback cb) {
  fireTimes.push_back(fireTime);
  singleCallbacks.push_back(cb);
}

int Mediator::getCurrentSamp() {
  return currentSamp;
}
