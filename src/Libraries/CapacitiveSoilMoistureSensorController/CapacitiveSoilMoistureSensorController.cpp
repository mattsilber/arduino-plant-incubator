#include "Arduino.h"
#include "CapacitiveSoilMoistureSensorController.h"

int CapacitiveSoilMoistureSensorController::getRawAnalogReading() {
  digitalWrite(_transitorBasePin, HIGH);

  delayMicroseconds(100);

  int analogReading = analogRead(_readPin);

  digitalWrite(_transitorBasePin, LOW);

  return analogReading;
}

double CapacitiveSoilMoistureSensorController::getMoisturePercentage() {
  return min(
    1.0,
    max(
      0.0,
      (getRawAnalogReading() - _analogMin) / (1.0 * max(1.0, (_analogMax - _analogMin)))
    )
  );
}
