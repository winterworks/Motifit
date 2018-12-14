#include "IQTTFunctions.h"
#include "config.h"

// variables to detect bypassers
String passCountDay = Time.format(Time.now(), "%Y-%m-%d");
long passCount = 0;
int checkDetectorInterval = 10;
int detectingStep = 0;
int detectingStepSerialCount = 0;

int minDetectionTime = 150;
int maxDetectionTime = 330;
// Used pins
int infraredDetector = A1;
int buttonInput = 2;
// Variable for setting the sleep time and lenght (7 hours for default)
int sleepNightSeconds = 60*60*7;
int bedTimeHour = 23;
// Callback for MQTT
void callback(char* topic, byte* payload, unsigned int length);

String topic = "mf-status";
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
        client.subscribe(topic);
    } else {
        Particle.publish("MQTT connection", "failed");
    }
}

void loop() {
    if (client.isConnected()) {
        client.loop();
    }
    checkBedTime();
    detect();

    delay(checkDetectorInterval);
}

void checkBedTime(){
    if (Time.format(Time.now(), "%H").toInt() == bedTimeHour) {
        System.sleep(SLEEP_MODE_DEEP,sleepNightSeconds);
    }
    // Optional: add an option to also disable the photon in the weekends
}

void detect(){
    // bool detecting = analogRead(infraredDetector) < 300;
    bool detecting = digitalRead(buttonInput);
    if (detecting) {
        if (detectingStepSerialCount < 5) {
            detectingStep++;
            if(detectingStep == minDetectionTime){
                increaseLogCount();
                detectingStepSerialCount++;
                // Debugging line for checking the passCount before sending it over MQTT
                publishValue();
            }
            if(detectingStep > maxDetectionTime){
                // Detected long enough restart it
                detectingStep = 0;
            }
        }
    } else {
        // Reset the detectingStep
        detectingStep = 0;
        detectingStepSerialCount = 0;
    }
}

void publishValue(){
    if (client.isConnected()) {
        client.publish(topic, String(passCount));
    }
}

void increaseLogCount(){
    if (passCountDay == Time.format(Time.now(), "%Y-%m-%d")) {
        // It's stil the same day increase the current count
        passCount++;
        
    } else {
        // The day has passed reset the count
        passCount = 0;
        passCountDay = Time.format(Time.now(), "%Y-%m-%d");
        // TODO make a log of the past days
    }
}
