#include <Arduino.h>
#include <CapacitiveSoilMoistureSensorController.h>
#include <WaterSolenoidValveRelayController.h>

WaterSolenoidValveRelayController valveController(
  6,
  CapacitiveSoilMoistureSensorController(A3, 5, 0, 1500),
  {
    minMoistureThreshold: -0.1,
    maxWateringDurationMs: 2500L,
    minWaterFrequencyMs: 10000 // 1000L * 60 * 60
  }
);

void setup() {
  Serial.begin(9600);
}

/**
 * ....
 */
void loop() {
  valveController.loop();

  delay(1000);
}
