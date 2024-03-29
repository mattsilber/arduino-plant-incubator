#include <LightController.h>

LightController lightController(4);

boolean lightOn = false;
long lightChangedAtMs = 0;

void setup() { }

/**
 * Toggle the light relay on and off, once per second
 */
void loop() {
  if (1000 < millis() - lightChangedAtMs) {
    lightChangedAtMs = millis();
    lightOn = !lightOn;
    
    lightController.setLightOn(lightOn);
  }

  delay(10);
}
