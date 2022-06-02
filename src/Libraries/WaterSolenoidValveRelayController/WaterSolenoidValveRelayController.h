#ifndef WaterSolenoidValveRelayController_h
#define WaterSolenoidValveRelayController_h

#include "Arduino.h" 
#include "CapacitiveSoilMoistureSensorController.h" 

struct WateringConfig {
  double minMoistureThreshold;
  long maxWateringDurationMs;
  long minWaterFrequencyMs;
};

class WaterSolenoidValveRelayController {

public:
  WaterSolenoidValveRelayController(
    int solenoidRelayPin,
    CapacitiveSoilMoistureSensorController moistureSensor,
    WateringConfig wateringConfig
  ): _solenoidRelayPin(solenoidRelayPin),
    _moistureSensor(moistureSensor),
    _wateringConfig(wateringConfig),
    _lastWateredAtMs(0) {
      pinMode(solenoidRelayPin, OUTPUT);
    };
  
  void loop();
  void attemptWatering(long durationMs);
  bool isMoistureThresholdMet();
  bool isFrequencyThresholdMet();

private:
  int _solenoidRelayPin;
  CapacitiveSoilMoistureSensorController _moistureSensor;
  WateringConfig _wateringConfig;
  long _lastWateredAtMs;

};

#endif
