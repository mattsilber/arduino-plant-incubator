#ifndef MotorizedLightController_h
#define MotorizedLightController_h

#include "Arduino.h" 
#include "EndstopController.h" 
#include "A4988MotorController.h" 
#include "LightController.h" 

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
  int trackLengthInMillimeters;
  int runDurationSeconds;
  int restartDelaySeconds;
};

class MotorizedLightController {

public:
  MotorizedLightController(String id, MotorizedLightSource motorizedLightSource): 
    _id(id),
    _motorizedLightSource(motorizedLightSource), 
    _state(searchingForHome), 
    _stateChangedAtMs(0), 
    _currentStepPosition(0), 
    _lastStepForwardAtMs(0) {};

  void loop();

private:
  String _id;

  MotorizedLightSource _motorizedLightSource;
  MotorizedLightState _state;
  long _stateChangedAtMs;
  int _currentStepPosition;
  long _lastStepForwardAtMs;

  void handleSearchingForHomeLoopCheck();
  void handleForwardMovementLoopCheck();
  void handleBackToHomeLoopCheck();
  void handleAwaitingRestartLoopCheck();
  void setMovingForwards();
};

#endif
