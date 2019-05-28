#include "Radar.h"
#include "Led.h"
#include "Sonar.h"
#include "config.h"

#define ANGLE_TO_PULSE(x) (map(x, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH))

Msg* msg;
bool rising = true;
//long time1;
//long time2;
Radar::Radar(SharedContext *pContext) {
  this->ledConn = new Led(L1_PIN);
  this->prox = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN);
  this->pContext = pContext;
}

void Radar::init(int period){
  Task::init(period);
  state = IDLE;
  pos = 90;
  //servo.write();
  servo.attach(SERVO_PWM_PIN);
}

void Radar::tick() {
 // time1 = millis();
  switch(state) {
    case IDLE:
        if (pContext->isWorking()) {
          ledConn->switchOn();
          pContext->setStopped(false);
          state = SCANNING;
       } 
      break;
      case SCANNING:
        //delay(2000);
        if (pContext -> isTracking()) {
      //    Logger.log("STOP");
          pContext->setWorking(false);
          pContext->setTracking(false);
          //pContext->setStopped(true);
          state = STOP;
        }
        else if(pContext->isStopped()) {
       //   Logger.log("OFF");
          pContext->setWorking(false);
          pContext->setStopped(true);
          pos = 90;
         // wr = map(pos, MIN_ANGLE, MAX_ANGLE, MIN_SERVO_T2, MAX_SERVO_T2);
          servo.write(ANGLE_TO_PULSE(pos));
          ledConn->switchOff();
          state = IDLE;
        }
        
    if(!pContext->isStopped()){
        if(pos < 180 && rising) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            // wr = map(pos, MIN_ANGLE, MAX_ANGLE, MIN_SERVO_T2, MAX_SERVO_T2);
            servo.write(ANGLE_TO_PULSE(pos));              // tell servo to go to position in variable 'pos'
            //delay(25);                       // waits 15ms for the servo to reach the position
            MsgService.sendMsg(String(pos) + ";" + String(prox->getDistance()));
            pos++;
            if(pos == 180){
              rising = false;
              }
           
        }
        if(pos > 0 && !rising) { // goes from 180 degrees to 0 degrees
         // wr = map(pos, MIN_ANGLE, MAX_ANGLE, MIN_SERVO_T2, MAX_SERVO_T2);
          servo.write(ANGLE_TO_PULSE(pos));              // tell servo to go to position in variable 'pos'
          //delay(25);                       // waits 15ms for the servo to reach the position
          MsgService.sendMsg(String(pos) + ";" + String(prox->getDistance()));    
          pos--;
          if(pos == 0){
            rising = true;
            }      
        }
       }
        break;
      case STOP:
        MsgService.sendMsg(String(pos) + ";" + String(prox->getDistance()));
          if (pContext->isWorking()) {
            pContext->setWorking(false);
            ledConn->switchOn();
            state = SCANNING;
          }
          else if(pContext->isStopped()) {
            pContext->setStopped(false);
            pos = 90;
            // wr = map(pos, MIN_ANGLE, MAX_ANGLE, MIN_SERVO_T2, MAX_SERVO_T2);
            servo.write(ANGLE_TO_PULSE(pos));
            ledConn->switchOff();
            state = IDLE;
          }
          break;
      }
    //  time2 = millis();
    //  Serial.println(String(time2 - time1));
}

//bool Radar::isStopped() {
//  Logger.log("Enter isStopped()");
//  if (state != SCANNING) {
//    stopped =  true;
//  }
//  if (MsgService.isMsgAvailable()) {
//      Logger.log("Msg available");
//      msg = MsgService.receiveMsg();
//      if (msg->getContent() == "s") {
//        Logger.log("STOP");
//        state = STOP;
//      }
//      else if(msg->getContent() == "o") {
//        Logger.log("OFF");
//        pos = 90;
//        servo.write(pos);
//        state = IDLE;
//      }
//      stopped = true;
//    }
//    Logger.log("Exiting isStopped(); stopped = " + String(stopped));
//    return stopped;
//  }


