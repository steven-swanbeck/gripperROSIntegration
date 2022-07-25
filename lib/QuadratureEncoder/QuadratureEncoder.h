#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

#define MAX_NUM_ENCODERS 12

class Encoders{
  public:  
    Encoders(byte pinA, byte pinB);
    static void interruptEncoder1(){
      if(Encoders::_instances[0] != NULL)
      Encoders::_instances[0]->encoderCount();
    }
    static void interruptEncoder2(){
      if(Encoders::_instances[1] != NULL)
      Encoders::_instances[1]->encoderCount();
    }
    static void interruptEncoder3(){
      if(Encoders::_instances[2] != NULL)
      Encoders::_instances[2]->encoderCount();
    }
    static void interruptEncoder4(){
      if(Encoders::_instances[3] != NULL)
      Encoders::_instances[3]->encoderCount();
    }
    static void interruptEncoder5(){
      if(Encoders::_instances[4] != NULL)
      Encoders::_instances[4]->encoderCount();
    }
    static void interruptEncoder6(){
      if(Encoders::_instances[5] != NULL)
      Encoders::_instances[5]->encoderCount();
    }
    static void interruptEncoder7(){
      if(Encoders::_instances[6] != NULL)
      Encoders::_instances[6]->encoderCount();
    }
    static void interruptEncoder8(){
      if(Encoders::_instances[7] != NULL)
      Encoders::_instances[7]->encoderCount();
    }
    static void interruptEncoder9(){
      if(Encoders::_instances[8] != NULL)
      Encoders::_instances[8]->encoderCount();
    }
    static void interruptEncoder10(){
      if(Encoders::_instances[9] != NULL)
      Encoders::_instances[9]->encoderCount();
    }
    static void interruptEncoder11(){
      if(Encoders::_instances[10] != NULL)
      Encoders::_instances[10]->encoderCount();
    }
    static void interruptEncoder12(){
      if(Encoders::_instances[11] != NULL)
      Encoders::_instances[11]->encoderCount();
    }
    
    void encoderCount();
    long getEncoderCount();
    void setEncoderCount(long);
    long getEncoderErrorCount();
    static Encoders *_instances[MAX_NUM_ENCODERS];
    
  private:
    static uint8_t _whichEncoder;
    uint8_t _encoderPINA;
    uint8_t _encoderPINB;
    volatile long _encoderCount = 0;
    volatile int _lastEncoded = 0;
    volatile long _encoderErrors = 0;
};

#endif
