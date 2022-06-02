#include <A4988MotorController.h>
#include <EndstopController.h>
#include <LightController.h>
#include <MotorizedLightController.h>
#include <WaterSolenoidValveRelayController.h>
#include <IncubationGroup.h>

IncubationGroup incubationGroups[] = {
  IncubationGroup(
    MotorizedLightController(
      "Light Over Plants",
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
      6,
      CapacitiveSoilMoistureSensorController(A3, 5, 0, 1500),
      {
        minMoistureThreshold: -0.1,
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
