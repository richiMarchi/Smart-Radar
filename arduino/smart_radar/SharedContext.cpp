#include "SharedContext.h"
#include "config.h"

SharedContext::SharedContext() {
    
}

  bool SharedContext::isStopped() {
    return stopped;
  }
  bool SharedContext::isTracking() {
    return tracking;
  }
  bool SharedContext::isWorking() {
    return working;
  }
  void SharedContext::setStopped(bool value) {
    stopped = value;
  }
  void SharedContext::setTracking(bool value) {
    tracking = value;
  }
  void SharedContext::setWorking(bool value) {
    working = value;
  }




