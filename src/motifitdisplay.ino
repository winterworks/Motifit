// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>

// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal.h>


long passCount = 0;
int checkDetectorInterval = 5000;
int infraredDetector = A1;
void callback(char* topic, byte* payload, unsigned int length);
MQTT client("iot.eclipse.org", 1883, callback);


// pinout on LCD [RS, EN, D4, D5, D6, D7];
// pin nums LCD  [ 4,  6, 11, 12, 13, 14];
LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);
String stairs="100";

void callback(char* topic, byte* payload, unsigned int length) {
    
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    Particle.publish("MQTT receive:", p);
    stairs=p;
}

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);
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
  
  lcd.clear();    
  lcd.setCursor(0, 0);
  lcd.print("Stairs:");
  lcd.setCursor(0,1);
  lcd.print(stairs);
  lcd.print(" :)");
  delay(checkDetectorInterval);    
}