#include "Arduino.h"
#include "MotorizedLightController.h"
#include "EndstopController.h"
#include "A4988MotorController.h"
#include "LightController.h"

void MotorizedLightController::loop() {
  switch (_state) {
    case searchingForHome:
      handleSearchingForHomeLoopCheck();
      break;
    case movingForwards:
      handleForwardMovementLoopCheck();
      break;
    case movingBackToHome:
      handleBackToHomeLoopCheck();
      break;
    case awaitingRestart:
      handleAwaitingRestartLoopCheck();
      break;
  }
}

void MotorizedLightController::handleSearchingForHomeLoopCheck() {
  Serial.println("Starting search for home for group: " + _id);
  
  while (!_motorizedLightSource.endstopHome.isReached()) {
    _motorizedLightSource.motorController.move(1, !_motorizedLightSource.clockwiseMovesForward);
  }

  Serial.println("Home found for group: " + _id);
    
  setMovingForwards();
}

void MotorizedLightController::handleForwardMovementLoopCheck() {
  long runDurationMs = millis() - _stateChangedAtMs;
  
  if ((_motorizedLightSource.runDurationSeconds * 1000) < runDurationMs) {
    Serial.println("Run duration expired for group: " + _id);

    _state = movingBackToHome;
    _stateChangedAtMs = millis();
    
    _motorizedLightSource.lightController.setLightOn(false);

    return;
  }
  
  if (_motorizedLightSource.endstopEnd.isReached()) {
    Serial.println("Forward endstop reached for group: " + _id);
    
    return;
  }

  double percentCompleted = (millis() - _stateChangedAtMs) / (_motorizedLightSource.runDurationSeconds * 1000.0);
    
  // TODO: Precalculate
  int totalStepsRequired = _motorizedLightSource.motorController.estimatedStepsPerTrack(
    _motorizedLightSource.millimetersPerMotorRevolution,
    _motorizedLightSource.trackLengthInMillimeters
  );

  Serial.println(
    "Group " + _id 
      + ": " 
      + String(percentCompleted, 3) 
      + " percent completed, " 
      + String(totalStepsRequired)
      + " total steps required, " 
      + String(_currentStepPosition) 
      + " current position"
  );

  int stepsThatShouldHaveBeenCompletedByNow = percentCompleted * totalStepsRequired;
  
  int stepsToMoveForward = stepsThatShouldHaveBeenCompletedByNow - _currentStepPosition;

  if (stepsToMoveForward < 1) {
    Serial.println("Forward movement not yet required for group: " + _id);
    
    return;
  }
  
  _currentStepPosition += stepsToMoveForward;
  _lastStepForwardAtMs = millis();
  
  _motorizedLightSource.motorController.move(stepsToMoveForward, _motorizedLightSource.clockwiseMovesForward);
}

void MotorizedLightController::handleBackToHomeLoopCheck() {
  while (!_motorizedLightSource.endstopHome.isReached()) {
    _currentStepPosition = max(0, _currentStepPosition - 1);
    _motorizedLightSource.motorController.move(1, !_motorizedLightSource.clockwiseMovesForward);
  }
  
  Serial.println("Home endstop reached for group: " + _id);
    
  _state = awaitingRestart;
  _stateChangedAtMs = millis();

  // Move some arbitrary number of steps off the home endstop so it's not pulled high
  _currentStepPosition = 20;
  _motorizedLightSource.motorController.move(_currentStepPosition, _motorizedLightSource.clockwiseMovesForward);
}

void MotorizedLightController::handleAwaitingRestartLoopCheck() {
  long sleptDurationMs = millis() - _stateChangedAtMs;

  if ((_motorizedLightSource.restartDelaySeconds * 1000) < sleptDurationMs) {
    setMovingForwards();
  }
}

void MotorizedLightController::setMovingForwards() {
  Serial.println("Beginning group forward movement sequence for group: " + _id);
    
  _state = movingForwards;
  _stateChangedAtMs = millis();

  _motorizedLightSource.lightController.setLightOn(true);
}