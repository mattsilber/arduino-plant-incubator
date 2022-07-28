#include <A4988MotorController.h>
#include <EndstopController.h>
#include <LightController.h>
#include <MotorizedLightController.h>
#include <CapacitiveSoilMoistureSensorController.h>
#include <WaterSolenoidValveRelayController.h>
#include <IncubationGroup.h>

IncubationGroup incubationGroups[] = {
  IncubationGroup(
    MotorizedLightController(
      "Plant Section A",
      {
        lightController: LightController(4),
        motorController: A4988MotorController(2, 3, 200),
        endstopHome: EndstopController(A0),
        endstopEnd: EndstopController(A1),
        clockwiseMovesForward: true,
        millimetersPerMotorRevolution: 25,
        trackLengthInMillimeters: 500,
        runDurationSeconds: 3600,
        restartDelaySeconds: 1800
      }
    ),
    WaterSolenoidValveRelayController(
      "Plants A, B, C",
      CapacitiveSoilMoistureSensorController(A2, 5, 334, 521, true),
      {
        solenoidRelayPin: 6,
        minMoistureThreshold: 0.45,
        maxWateringDurationMs: 2500L,
        minWaterFrequencyMs: 1000L * 60 * 60 * 1
      }
    )
  )
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (IncubationGroup group : incubationGroups) {    
    group.loop();
  }

  delay(1000);
}
