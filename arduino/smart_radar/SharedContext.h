#ifndef __SHAREDCONTEXT__
#define __SHAREDCONTEXT__

#include "ProximitySensor.h"

class SharedContext {
public:
  SharedContext();
  bool isStopped();
  bool isTracking();
  bool isWorking();
  void setStopped(bool value);
  void setTracking(bool value);
  void setWorking(bool value);
  
private:
  volatile bool stopped = false;
  volatile bool tracking = false;
  volatile bool working = false;
 };

#endif
