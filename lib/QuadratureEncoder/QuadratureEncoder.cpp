#include "QuadratureEncoder.h"
#include <EnableInterrupt.h>

// initialize all instance of encoder to null.

Encoders *Encoders::_instances[MAX_NUM_ENCODERS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
uint8_t Encoders::_whichEncoder = 0;
Encoders::Encoders(byte pinA, byte pinB){
   _encoderPINA = pinA;
   _encoderPINB = pinB;
   pinMode(_encoderPINA, INPUT_PULLUP);  
   pinMode(_encoderPINB, INPUT_PULLUP);
   _whichEncoder++;
   switch(_whichEncoder){
    case 1:
        enableInterrupt(_encoderPINB, interruptEncoder1, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder1, CHANGE);  
        _instances[0] = this;
        break;
     case 2:
        enableInterrupt(_encoderPINB, interruptEncoder2, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder2, CHANGE);  
        _instances[1] = this;
        break;
     case 3:
        enableInterrupt(_encoderPINB, interruptEncoder3, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder3, CHANGE); 
        _instances[2] = this; 
        break;
     case 4:
        enableInterrupt(_encoderPINB, interruptEncoder4, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder4, CHANGE);  
        _instances[3] = this;
        break;
     case 5:
        enableInterrupt(_encoderPINB, interruptEncoder5, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder5, CHANGE);  
        _instances[4] = this;
        break;
     case 6:
        enableInterrupt(_encoderPINB, interruptEncoder6, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder6, CHANGE);  
        _instances[5] = this;
        break;
     case 7:
        enableInterrupt(_encoderPINB, interruptEncoder7, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder7, CHANGE);  
        _instances[6] = this;
        break;
     case 8:
        enableInterrupt(_encoderPINB, interruptEncoder8, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder8, CHANGE);  
        _instances[7] = this;
        break;
     case 9:
        enableInterrupt(_encoderPINB, interruptEncoder9, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder9, CHANGE);  
        _instances[8] = this;
        break;
     case 10:
        enableInterrupt(_encoderPINB, interruptEncoder10, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder10, CHANGE);  
        _instances[9] = this;
        break;
     case 11:
        enableInterrupt(_encoderPINB, interruptEncoder11, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder11, CHANGE);  
        _instances[10] = this;
        break;
     case 12:
        enableInterrupt(_encoderPINB, interruptEncoder12, CHANGE);
        enableInterrupt(_encoderPINA,  interruptEncoder12, CHANGE);  
        _instances[11] = this;
        break;
   }
}




void Encoders::encoderCount(){
  int EncoderPhaseA = digitalRead(this->_encoderPINA);  // MSB
  int EncoderPhaseB = digitalRead(this->_encoderPINB);  // LSB

  int currentEncoded = (EncoderPhaseA << 1) | EncoderPhaseB;
  int sum = (this->_lastEncoded << 2) | currentEncoded;
  switch(sum){
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
      this->_encoderCount--;
      break;
    case 0b0010:
    case 0b1011:
    case 0b1101:
    case 0b0100:
      this->_encoderCount++;
      break;
    default:
      this->_encoderErrors++;
      break;
  }
  this->_lastEncoded = currentEncoded;
}

long Encoders::getEncoderCount(){
  return _encoderCount;
}
void Encoders::setEncoderCount(long setEncoderVal){
  this->_encoderCount = setEncoderVal;
}

long Encoders::getEncoderErrorCount(){
  return _encoderErrors;
}
