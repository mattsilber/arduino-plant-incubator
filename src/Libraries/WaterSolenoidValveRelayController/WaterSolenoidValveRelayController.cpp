#include "Arduino.h"
#include "CapacitiveSoilMoistureSensorController.h"
#include "WaterSolenoidValveRelayController.h"

void WaterSolenoidValveRelayController::loop() {
  if (isFrequencyThresholdMet() && isMoistureThresholdMet()) {
    Serial.println(
      "Attempting to water group: "
        + String(_id)
    );

    attemptWatering(_wateringConfig.maxWateringDurationMs);
  }
}

void WaterSolenoidValveRelayController::attemptWatering(long durationMs) {
  if (!isMoistureThresholdMet()) {
    Serial.println(
      "Moisture threshold not met. Ignoring watering request for group: "
        + String(_id)
    );

    return;
  }

  if (!isFrequencyThresholdMet()) {
    Serial.println(
      "Frequency threshold not met. Ignoring watering request for group: "
        + String(_id)
    );

    return;
  }

  long waterDurationMs = min(_wateringConfig.maxWateringDurationMs, durationMs);
    
  Serial.println(
    "Starting watering cycle for group: "
      + String(_id)
      + " for "
      + String(waterDurationMs)
      + "ms"
  );

  digitalWrite(_wateringConfig.solenoidRelayPin, HIGH);

  delay(waterDurationMs);

  digitalWrite(_wateringConfig.solenoidRelayPin, LOW);

  _lastWateredAtMs = millis();

  Serial.println(
    "Completed watering cycle for group: "
      + String(_id)
  );
}

bool WaterSolenoidValveRelayController::isMoistureThresholdMet() {
  return _wateringConfig.minMoistureThreshold < _moistureSensor.getMoisturePercentage();
}

bool WaterSolenoidValveRelayController::isFrequencyThresholdMet() {
  return _wateringConfig.minWaterFrequencyMs < millis() - _lastWateredAtMs;
}