#ifndef CapacitiveSoilMoistureSensorController_h
#define CapacitiveSoilMoistureSensorController_h

#include "Arduino.h" 

class CapacitiveSoilMoistureSensorController {

public:
  /*
  To find `analogMin`, read the analog value of the sensor in the soil at its dryest.
  To find `analogMax`, read the analog value of the sensor in the soil at its wettest.
  */
  CapacitiveSoilMoistureSensorController(
    int readPin,
    int transitorBasePin,
    int analogMin,
    int analogMax
  ): _readPin(readPin), 
    _transitorBasePin(transitorBasePin), 
    _analogMin(analogMin), 
    _analogMax(analogMax) {
    pinMode(readPin, INPUT);
    pinMode(transitorBasePin, OUTPUT);
  };
  
  int getRawAnalogReading();
  double getMoisturePercentage();

private:
  int _readPin;
  int _transitorBasePin;
  int _analogMin;
  int _analogMax;

};

#endif
