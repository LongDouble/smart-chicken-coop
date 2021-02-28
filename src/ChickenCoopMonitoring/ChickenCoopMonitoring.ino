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
float temperature;

Lamp lamp(LAMP_PIN);
WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);
DateTime future, now;

RTC_DS3231 rtc;

//Daylight sensor objects
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
uint32_t lum;
uint16_t ir, full;
float lux;
bool daylight;

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

  future = rtc.now();
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
  lamp.set(0);

  //initialize echo/trigger pins for food and water
  pinMode(FOOD_TRIG, OUTPUT);
  pinMode(FOOD_ECHO, INPUT);
  pinMode(WATER_TRIG, OUTPUT);
  pinMode(WATER_ECHO, INPUT);
}

// the loop routine runs over and over again forever:
void loop()
{

  // Timestamp / Temperature
  now = rtc.now();

  // Food / Water Tracking

  // Daylight

  // Heat Lamp

  // SD (Format: Timestamp, Food(g), Water(mL), Temp (C), Daylight (Y/N))
  SD.begin(SD_PIN);
  // if now.hour() == future.hour()
  if (now.hour() >= future.hour())
  {
    future = now + TimeSpan(0, 1, 0, 0);
    // Log data to SD card
  }
  //DateTime future = now.hour() + an hour

  delay(1000); // delay in between reads for stability
}
