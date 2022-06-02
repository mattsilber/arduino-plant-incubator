#ifndef WaterSolenoidValveRelayController_h
#define WaterSolenoidValveRelayController_h

#include "Arduino.h" 
#include "CapacitiveSoilMoistureSensorController.h" 

struct WateringConfig {
  int solenoidRelayPin;
  double minMoistureThreshold;
  long maxWateringDurationMs;
  long minWaterFrequencyMs;
};

class WaterSolenoidValveRelayController {

public:
  WaterSolenoidValveRelayController(
    String id,
    CapacitiveSoilMoistureSensorController moistureSensor,
    WateringConfig wateringConfig
  ): _id(id),
    _moistureSensor(moistureSensor),
    _wateringConfig(wateringConfig),
    _lastWateredAtMs(0) {
      pinMode(wateringConfig.solenoidRelayPin, OUTPUT);
    };
  
  void loop();
  void attemptWatering(long durationMs);
  bool isMoistureThresholdMet();
  bool isFrequencyThresholdMet();

private:
  String _id;

  CapacitiveSoilMoistureSensorController _moistureSensor;
  WateringConfig _wateringConfig;

  long _lastWateredAtMs;

};

#endif
