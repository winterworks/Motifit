#include "IQTTFunctions.h"
#include "config.h"

long passCount = 0;
int checkDetectorInterval = 50;
int detectingStep = 0;
int detectingStepSerialCount = 0;
int infraredDetector = A1;
int buttonInput = 2;
void callback(char* topic, byte* payload, unsigned int length);

MQTT client("m23.cloudmqtt.com", 13776, callback);

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    Particle.publish("MQTT Passcount:", p);
}


void setup() {
    pinMode(buttonInput, INPUT);
  
     Particle.publish("MQTT connection", "Connection to: m23.cloudmqtt.com");
    client.connect("mf_" + String(Time.now()), username, passoword);

    if (client.isConnected()) {
        Particle.publish("MQTT connection", "success");
        client.subscribe("mf-status");
    } else {
        Particle.publish("MQTT connection", "failed");
    }
}

void loop() {
    if (client.isConnected()) {
        client.loop();
    }
    
    // bool detecting = analogRead(infraredDetector) < 300;
    bool detecting = digitalRead(buttonInput);
    if (detecting) {
        if (detectingStepSerialCount < 10) {
            detectingStep++;
            if(detectingStep > 5){
                passCount++;
                detectingStepSerialCount++;
                // Debugging line for checking the passCount before sending it over MQTT
                detectingStep = 0;
                sendValue();
            }
        }
    } else {
        // Reset the detectingStep
        detectingStep = 0;
        detectingStepSerialCount = 0;
    }
    delay(checkDetectorInterval);
}

void sendValue(){
    if (client.isConnected()) {
        client.publish("mf-status", String(passCount));
    }
}