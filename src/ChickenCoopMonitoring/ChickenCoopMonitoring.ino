/*
  Chicken Coop Monitoring System

*/
#include <Lamp.h>
#include <WarningLights.h>

#define GREEN_PIN 2
#define YELLOW_PIN 3
#define RED_PIN 4
#define LAMP_PIN 5

Lamp lamp(LAMP_PIN);
WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // set lamp to highest power
  lamp.set(5);

  // turn on warning lights
  warninglights.green();
  delay(2000);
  
  warninglights.yellow();
  delay(2000);
  
  warninglights.red();
  delay(2000);
  
  warninglights.off();
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  delay(1);        // delay in between reads for stability
}
