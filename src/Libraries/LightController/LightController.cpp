#include "Arduino.h"
#include "LightController.h"

LightController::LightController(int lightPin) {
  pinMode(lightPin, OUTPUT);
  
  _lightPin = lightPin;
}

void LightController::setLightOn(bool on) {
  digitalWrite(_lightPin, on ? HIGH : LOW);
}
