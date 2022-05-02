#ifndef LightController_h
#define LightController_h

#include "Arduino.h" 

class LightController {

public:
  LightController(int lightPin);
  
  void setLightOn(bool on);

private:
  int _lightPin;

};

#endif
