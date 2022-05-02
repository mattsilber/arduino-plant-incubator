#include "Arduino.h"
#include "A4988MotorController.h"

A4988MotorController::A4988MotorController(int stepPin, int directionPin, int stepsPerRevolution) {
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  
  _stepPin = stepPin;
  _directionPin = directionPin;
  _stepsPerRevolution = stepsPerRevolution;
}

void A4988MotorController::move(int steps, bool clockwise) {
  digitalWrite(_directionPin, clockwise ? HIGH : LOW);

  for (int step = 0; step < steps; step++) {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(1000);
    
    digitalWrite(_stepPin, LOW);
    delayMicroseconds(1000);
  }
}

int A4988MotorController::estimatedStepsPerTrack(int millimetersPerRevolution, int trackLengthInMillimeters) {
  return _stepsPerRevolution * (trackLengthInMillimeters / (millimetersPerRevolution * 1.0));
}