#ifndef _MEDIATOR_H_
#define _MEDIATOR_H_

#include <stdlib.h>
#include <vector>

typedef void (*Callback)();

class Mediator
{
  int currentSamp;
  // Periods for callbacks in terms of samples
  std::vector<int> periods;
  // Periodic callbacks
  std::vector<Callback> callbacks;
  // Fire times for one-time callbacks in terms of samples
  std::vector<int> fireTimes;
  // one-time callbacks
  std::vector<Callback> singleCallbacks;

  public:
    Mediator ();
    void updateCount(int sampleCount);
    int getCurrentSamp();
    void registerCallback(int period, Callback cb);
    void registerSingleCallback(int fireTime, Callback cb);
};

#endif
