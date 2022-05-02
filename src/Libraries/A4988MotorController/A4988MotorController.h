#ifndef A4988MotorController_h
#define A4988MotorController_h

#include "Arduino.h" 

/**
 * Setup using an A4988 without any of the step-altering pins connected.
 */
class A4988MotorController {

public:
  /**
  * stepsPerRevolution = motor-step-angle / 360; 
  * e.g. 1.8 degrees = 200 steps/revolution
  */
  A4988MotorController(int stepPin, int directionPin, int stepsPerRevolution);
  
  void move(int steps, bool clockwise);
  
  int estimatedStepsPerTrack(int millimetersPerRevolution, int trackLengthInMillimeters);

private:
  int _stepPin;
  int _directionPin;
  int _stepsPerRevolution;

};

#endif
