#ifndef EndstopController_h
#define EndstopController_h

#include "Arduino.h" 

class EndstopController {

public:
  EndstopController(int ncPin);
  
  bool isReached();

private:
  int _ncPin;

};

#endif
