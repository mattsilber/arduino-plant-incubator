#include "Arduino.h"
#include "CapacitiveSoilMoistureSensorController.h"
#include "WaterSolenoidValveRelayController.h"

void WaterSolenoidValveRelayController::loop() {
  if (isFrequencyThresholdMet() && isMoistureThresholdMet()) {
    Serial.println(
      "Attempting to water pin: "
        + String(_solenoidRelayPin)
    );

    attemptWatering(_wateringConfig.maxWateringDurationMs);
  }
}

void WaterSolenoidValveRelayController::attemptWatering(long durationMs) {
  if (!isMoistureThresholdMet()) {
    Serial.println(
      "Moisture threshold not met. Ignoring watering request for pin: "
        + String(_solenoidRelayPin)
    );

    return;
  }

  if (!isFrequencyThresholdMet()) {
    Serial.println(
      "Frequency threshold not met. Ignoring watering request for pin: "
        + String(_solenoidRelayPin)
    );

    return;
  }

  long waterDurationMs = min(_wateringConfig.maxWateringDurationMs, durationMs);
    
  Serial.println(
    "Starting watering cycle for pin: " 
      + String(_solenoidRelayPin) 
      + " for "
      + String(waterDurationMs)
      + "ms"
  );

  digitalWrite(_solenoidRelayPin, HIGH);

  delay(waterDurationMs);

  digitalWrite(_solenoidRelayPin, LOW);

  _lastWateredAtMs = millis();

  Serial.println(
    "Completed watering cycle for pin: "
      + String(_solenoidRelayPin)
  );
}

bool WaterSolenoidValveRelayController::isMoistureThresholdMet() {
  return _wateringConfig.minMoistureThreshold < _moistureSensor.getMoisturePercentage();
}

bool WaterSolenoidValveRelayController::isFrequencyThresholdMet() {
  return _wateringConfig.minWaterFrequencyMs < millis() - _lastWateredAtMs;
}