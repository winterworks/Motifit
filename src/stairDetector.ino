#include <MQTT.h>

int led0 = D1;
int infraredDetector = A1;
void callback(char* topic, byte* payload, unsigned int length);
MQTT client("iot.eclipse.org", 1883, callback);

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    Particle.publish("status", p);
    if(strcmp(p, "1")){
      digitalWrite(led0, HIGH);
    } else {
      digitalWrite(led0, LOW);
    }
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  pinMode(led0, OUTPUT);

  // MQTT code from: https://github.com/hirotakaster/MQTT
  // connect to the server(unique id by Time.now()) and subscribe
  client.connect("mf_" + String(Time.now()));
  if (client.isConnected()) {
      Particle.publish("client connected", "yes");
      client.subscribe("mf-status");
  }
}

void loop() {
  // Publis the detected result to the Particle console every 3 seconds
   String detecting = String(analogRead(infraredDetector) > 300);
  if (client.isConnected()) {
      client.loop();
      client.publish("mf-status", detecting);
  }
   delay(5000);
}
