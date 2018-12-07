#include "IQTTFunctions.h"

long passCount = 0;
int checkDetectorInterval = 5000;
int infraredDetector = A1;
void callback(char* topic, byte* payload, unsigned int length);
MQTT client("iot.eclipse.org", 1883, callback);

void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    Particle.publish("MQTT receive:", p);
}

void setup() {
  // MQTT code from: https://github.com/hirotakaster/MQTT
  // connect to the server(unique id by Time.now()) and subscribe
  client.connect("mf_" + String(Time.now()));
  if (client.isConnected()) {
      Particle.publish("client connected", "yes");
      client.subscribe("mf-status");
  }
}

void loop() {
  Particle.publish("Input value:", String(analogRead(infraredDetector)));
  // Publis the detected result to the Particle console
  bool detecting = analogRead(infraredDetector) > 300;
  if (client.isConnected()) {
      client.loop();
      if (detecting) {
          passCount++;
      }
      client.publish("mf-status", String(passCount));
  }
   delay(checkDetectorInterval);
}
