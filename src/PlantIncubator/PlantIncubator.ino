#include <A4988MotorController.h>
#include <EndstopController.h>
#include <LightController.h>

enum MotorizedLightState {
  searchingForHome,
  movingForwards,
  movingBackToHome,
  awaitingRestart
};

struct MotorizedLightSource {
  LightController lightController;
  A4988MotorController motorController;
  EndstopController endstopHome;
  EndstopController endstopEnd;
  boolean clockwiseMovesForward;
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
      lightController: LightController(4),
      motorController: A4988MotorController(2, 3, 200),
      endstopHome: EndstopController(A0),
      endstopEnd: EndstopController(A1),
      clockwiseMovesForward: true,
      millimetersPerMotorRevolution: 25
    },
    state: searchingForHome,
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
  for (IncubationGroup &group : incubationGroups) {    
    switch (group.state) {
      case searchingForHome:
        handleSearchingForHomeLoopCheck(&group);
        break;
      case movingForwards:
        handleForwardMovementLoopCheck(&group);
        break;
      case movingBackToHome:
        handleBackToHomeLoopCheck(&group);
        break;
      case awaitingRestart:
        handleAwaitingRestartLoopCheck(&group);
        break;
    }
  }

  delay(1000);
}

void handleSearchingForHomeLoopCheck(IncubationGroup *group) {
  Serial.println("Starting search for home for group: " + group->id);
  
  while (!group->motorizedLightSource.endstopHome.isReached()) {
    group->motorizedLightSource.motorController.move(1, !group->motorizedLightSource.clockwiseMovesForward);
  }

  Serial.println("Home found for group: " + group->id);
    
  setMovingForwards(group);
}

void handleForwardMovementLoopCheck(IncubationGroup *group) {
  long runDurationMs = millis() - group->stateChangedAtMs;
  
  if ((group->runDurationSeconds * 1000) < runDurationMs) {
    Serial.println("Run duration expired for group: " + group->id);

    group->state = movingBackToHome;
    group->stateChangedAtMs = millis();
    
    group->motorizedLightSource.lightController.setLightOn(false);

    return;
  }
  
  if (group->motorizedLightSource.endstopEnd.isReached()) {
    Serial.println("Forward endstop reached for group: " + group->id);
    
    return;
  }

  double percentCompleted = (millis() - group->stateChangedAtMs) / (group->runDurationSeconds * 1000.0);
    
  // TODO: Precalculate
  int totalStepsRequired = group->motorizedLightSource.motorController.estimatedStepsPerTrack(
    group->motorizedLightSource.millimetersPerMotorRevolution,
    group->trackLengthInMillimeters
  );

  Serial.println("Group " + group->id + ": " + percentCompleted + " percent completed, " + totalStepsRequired + " total steps required, " + group->currentStepPosition + " current position");

  int stepsThatShouldHaveBeenCompletedByNow = percentCompleted * totalStepsRequired;
  
  int stepsToMoveForward = stepsThatShouldHaveBeenCompletedByNow - group->currentStepPosition;

  if (stepsToMoveForward < 1) {
    Serial.println("Forward movement not yet required for group: " + group->id);
    
    return;
  }
  
  group->currentStepPosition += stepsToMoveForward;
  
  group->motorizedLightSource.motorController.move(stepsToMoveForward, group->motorizedLightSource.clockwiseMovesForward);
  group->lastStepForwardAtMs = millis();
}

void handleBackToHomeLoopCheck(IncubationGroup *group) {
  while (!group->motorizedLightSource.endstopHome.isReached()) {
    group->currentStepPosition = max(0, group->currentStepPosition - 1);
    group->motorizedLightSource.motorController.move(1, !group->motorizedLightSource.clockwiseMovesForward);
  }
  
  Serial.println("Home endstop reached for group: " + group->id);
    
  group->state = awaitingRestart;
  group->stateChangedAtMs = millis();

  // Move some arbitrary number of steps off the home endstop so it's not pulled high
  group->currentStepPosition = 20;
  group->motorizedLightSource.motorController.move(20, group->motorizedLightSource.clockwiseMovesForward);
}

void handleAwaitingRestartLoopCheck(IncubationGroup *group) {
  long sleptDurationMs = millis() - group->stateChangedAtMs;

  if ((group->restartDelaySeconds * 1000) < sleptDurationMs) {
    setMovingForwards(group);
  }
}

void setMovingForwards(IncubationGroup *group) {
  Serial.println("Beginning group forward movement sequence for group: " + group->id);
    
  group->state = movingForwards;
  group->stateChangedAtMs = millis();

  group->motorizedLightSource.lightController.setLightOn(true);
}
