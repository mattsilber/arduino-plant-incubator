#ifndef IncubationGroup_h
#define IncubationGroup_h

#include "Arduino.h" 
#include "EndstopController.h" 
#include "A4988MotorController.h" 
#include "LightController.h" 
#include "MotorizedLightController.h"
#include "WaterSolenoidValveRelayController.h"

class IncubationGroup {

public:
  IncubationGroup(
  	MotorizedLightController motorizedLightController,
    WaterSolenoidValveRelayController wateringController
  ): _motorizedLightController(motorizedLightController), 
    _wateringController(wateringController) {};

  void loop();

private:
  MotorizedLightController _motorizedLightController;
  WaterSolenoidValveRelayController _wateringController;
};

#endif
