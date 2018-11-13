#include <MQTT.h>

void callback(char* topic, byte* payload, unsigned int length);
MQTT client("iot.eclipse.org", 1883, callback);

// MQTT code from: https://github.com/hirotakaster/MQTT
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    Particle.publish("status", p);
    switch (topic) {
      case mf-count:
        // TODO count received, show this on the display
        // display.show(p);
        break;
    }
}

void setupMqtt(bool hasDisplay){
  client.connect("mf_" + String(Time.now()));
  if (client.isConnected()) {
      if (hasDisplay) {
      client.subscribe("mf-count");
      }
  }
}
