#include <CapacitiveSoilMoistureSensorController.h>

CapacitiveSoilMoistureSensorController moistureSensorController(A2, 5, 0, 1500, true);

void setup() {
  Serial.begin(9600);
}

/**
 * Read and print the moisture level every second
 */
void loop() {
  Serial.println(
    "Moisture percentage: " 
      + String(moistureSensorController.getMoisturePercentage(), 3)
      + ", raw value: "
      + String(moistureSensorController.getRawAnalogReading())
  );

  delay(1000);
}
