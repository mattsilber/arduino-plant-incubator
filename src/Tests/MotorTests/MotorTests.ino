#include <A4988MotorController.h>
#include <EndstopController.h>
#include <LightController.h>

A4988MotorController motorController(2, 3, 200);
EndstopController endstopHome(A0);
EndstopController endstopEnd(A1);

boolean movingClockwise = true;

void setup() { }

/**
 * Bounce back and forth between the endstops, moving at 10 steps / second
 */
void loop() {
  if (movingClockwise) {
    if (endstopEnd.isReached()) {
      movingClockwise = false;
    }
    else {
      motorController.move(1, true);
    }
  }
  else {
    if (endstopHome.isReached()) {
      movingClockwise = true;
    }
    else {
      motorController.move(1, false);
    }
  }

  delay(10);
}
