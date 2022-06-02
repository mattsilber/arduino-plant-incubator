#include "Arduino.h"
#include "IncubationGroup.h"
#include "EndstopController.h"
#include "A4988MotorController.h"
#include "LightController.h"
#include "WaterSolenoidValveRelayController.h"

void IncubationGroup::loop() {
  _motorizedLightController.loop();
  _wateringController.loop();
}