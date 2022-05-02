#include <A4988MotorController.h>
#include <EndstopController.h>
#include <LightController.h>

enum MotorizedLightState {
  awaitingStart,
  movingForwards,
  movingBackToHome
};

struct MotorizedLightSource {
  LightController lightController;
  A4988MotorController motorController;
  EndstopController endstopHome;
  EndstopController endstopEnd;
  bool clockwiseMovesForward;
  int millimetersPerMotorRevolution;
};

struct IncubationGroup {
  String id;
  MotorizedLightSource motorizedLightSource;
  MotorizedLightState state;
  int trackLengthInMillimeters;
  int runDurationSeconds;
  int restartDelaySeconds;
  long stateChangedAtMs;
  int currentStepPosition;
  long lastStepForwardAtMs;
};

IncubationGroup incubationGroups[] = {
  { 
    id: "Group 1",
    motorizedLightSource: {
      lightController: LightController(1),
      motorController: A4988MotorController(2, 3, 200),
      endstopHome: EndstopController(A0),
      endstopEnd: EndstopController(A1),
      clockwiseMovesForward: true,
      millimetersPerMotorRevolution: 25
    },
    state: awaitingStart,
    trackLengthInMillimeters: 500,
    runDurationSeconds: 3600,
    restartDelaySeconds: 1800,
    stateChangedAtMs: -1,
    currentStepPosition: 0,
    lastStepForwardAtMs: -1
  }  
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (IncubationGroup group : incubationGroups) {    
    switch (group.state) {
      case awaitingStart:
        handleAwaitingStartLoopCheck(group);
        break;
      case movingForwards:
        handleForwardMovementLoopCheck(group);
        break;
      case movingBackToHome:
        handleBackToHomeLoopCheck(group);
        break;
    }
  }

  delay(1000);
}

void handleAwaitingStartLoopCheck(IncubationGroup group) {
  long sleptDurationMs = millis() - group.stateChangedAtMs;

  if (sleptDurationMs < group.restartDelaySeconds / 1000) {
    Serial.println("Beginning group forward movement sequence for group: " + group.id);

    group.state = movingForwards;
    group.stateChangedAtMs = millis();

    group.motorizedLightSource.lightController.setLightOn(true);
  }
}

void handleForwardMovementLoopCheck(IncubationGroup group) {
  long runDurationMs = millis() - group.stateChangedAtMs;
  
  if (group.runDurationSeconds / 1000 < runDurationMs) {
    Serial.println("Run duration expired for group: " + group.id);

    group.state = movingBackToHome;
    group.stateChangedAtMs = millis();
    
    group.motorizedLightSource.lightController.setLightOn(false);

    return;
  }
  
  if (group.motorizedLightSource.endstopEnd.isReached()) {
    Serial.println("Forward endstop reached for group: " + group.id);
    
    return;
  }

  double percentCompleted = (millis() - group.stateChangedAtMs) / group.runDurationSeconds;

  // TODO: Precalculate
  int totalStepsRequired = group.motorizedLightSource.motorController.estimatedStepsPerTrack(
    group.motorizedLightSource.millimetersPerMotorRevolution,
    group.trackLengthInMillimeters
  );

  int stepsThatShouldHaveBeenCompletedByNow = percentCompleted * totalStepsRequired;
  
  int stepsToMoveForward = stepsThatShouldHaveBeenCompletedByNow - group.currentStepPosition;

  if (stepsToMoveForward < 1) {
    Serial.println("Forward movement not yet required for group: " + group.id);
    
    return;
  }
  
  group.currentStepPosition += stepsToMoveForward;
  
  group.motorizedLightSource.motorController.move(stepsToMoveForward, group.motorizedLightSource.clockwiseMovesForward);
  group.lastStepForwardAtMs = millis();
}

void handleBackToHomeLoopCheck(IncubationGroup group) {
  if (group.motorizedLightSource.endstopHome.isReached()) {
    handleHomeEndstopReached(group);
    
    return;
  }

  for (int stepsMovedBackwards = 0; stepsMovedBackwards < group.currentStepPosition; stepsMovedBackwards++) {
    if (group.motorizedLightSource.endstopHome.isReached()) {
      handleHomeEndstopReached(group);
    
      return;
    }
    
    group.motorizedLightSource.motorController.move(1, !group.motorizedLightSource.clockwiseMovesForward);
  }  
}

void handleHomeEndstopReached(IncubationGroup group) {
  Serial.println("Home endstop reached for group: " + group.id);
    
  group.state = awaitingStart;
  group.stateChangedAtMs = millis();
}
