#ifndef __RADAR__
#define __RADAR__

#include "Arduino.h"
#include "ServoTimer2.h"
#include "SharedContext.h"
#include "Task.h"
#include "Light.h"
#include "ProximitySensor.h"
#include "MsgService.h"
#include "Logger.h"

class Radar: public Task {
  public:
    Radar(SharedContext *pContext);
    void init(int period);
    void tick();
  private:
    SharedContext* pContext;
    Light* ledConn;
    ProximitySensor* prox;
    ServoTimer2 servo;
    int pos;
    int wr;
    enum {IDLE, SCANNING, STOP} state;
};
#endif
