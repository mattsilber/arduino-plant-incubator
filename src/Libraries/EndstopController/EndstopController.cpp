#include "Arduino.h"
#include "EndstopController.h"

EndstopController::EndstopController(int ncPin) {
  pinMode(ncPin, INPUT);
  
  _ncPin = ncPin;
}

bool EndstopController::isReached() {
  return digitalRead(_ncPin) != LOW;
}
