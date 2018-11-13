#include <MQTT.h>
#include "IQTTFunctions.h"

int led0 = D1;
int infraredDetector = A1;
bool hasDisplay;
bool hasDetector;

int peopleCount = 0;
int checkDetectorInterval = 5000;

void setup() {
  Serial.begin(9600);
  Serial.println("started");
  hasDisplay = false; // Detect if this photon has a display
  hasDetector = true; // Detect if this photon has a detector

  setupMqtt(hasDisplay);
}

void loop() {
  // Read the infrared detector and convert result to a bool
  bool detecting = analogRead(infraredDetector) > 300;
  if (detecting) {
    peopleCount++;
  }
  // Particle.publish("read", String(analogRead(infraredDetector)));
  if (client.isConnected()) {
      client.loop();
      client.publish("mf-count", String(peopleCount));
  }
  delay(checkDetectorInterval);
}
