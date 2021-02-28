/*
  Chicken Coop Monitoring System

*/
#include <Lamp.h>
#include <WarningLights.h>
#include <RTClib.h>
#include <SD.h>
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

#define GREEN_PIN 2
#define YELLOW_PIN 3
#define RED_PIN 4
#define LAMP_PIN 5

#define SD_PIN 10

#define FOOD_TRIG 14
#define FOOD_ECHO 15
#define WATER_TRIG 16
#define WATER_ECHO 17

File myFile;
bool headerFlag = false;

Lamp lamp(LAMP_PIN);
WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
RTC_DS3231 rtc;

// the setup routine runs once when you press reset:
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

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
void loop()
{

  SD.begin(SD_PIN); // Initializes the SD

  // Timestamp / Temperature

  // Food / Water Tracking

  // Daylight

  // Heat Lamp

  // SD

  delay(1000); // delay in between reads for stability
}
