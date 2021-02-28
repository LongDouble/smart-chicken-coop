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
float temperature;
int lampLevel;

Lamp lamp(LAMP_PIN);
WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);
DateTime future, now, dim_track;

RTC_DS3231 rtc;

//Daylight sensor objects
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
uint32_t lum;
uint16_t ir, full;
float lux;
bool daylight;

//Food/water objects;
float water_mL, food_grams;
float distance_away = 3.1; //
float radius = 3;
float height = 13.6;
float density = 0.82844; //density of chicken feed g/mL

//Configuring gain and integration time on sensor
void configureSensor(void)
{
  //setting gain on the TSL2591 to average lighting setting
  tsl.setGain(TSL2591_GAIN_MED); // 25x gain

  //setting duration for Lux integration
  tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
}

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
  temperature = rtc.getTemperature();

  // Food Tracking

  // Water Tracking
  //declare mL, distance, and timeSignal
  float water_distance;
  double water_timeSignal;

  //prepare clean trig pulse with writing low for 5us
  digitalWrite(WATER_TRIG, LOW);
  delayMicroseconds(5);

  //datasheet requires at least 10us of HIGH pulse
  digitalWrite(WATER_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(WATER_TRIG, LOW);

  //read length of HIGH pulse in microseconds
  water_timeSignal = pulseIn(WATER_ECHO, HIGH);

  //use time to calculate distance in cm (speed of sound ~340m/s = 0.034cm/us, halve time taken to get one length)
  water_distance = water_timeSignal * 0.034 / 2;

  //V = L*pi*(r^2)
  water_mL = (height + distance_away - water_distance) * (3.14159 * radius * radius);

  // Daylight
  //Read 32 bits from both sensors in TSL2591, top 16 bits from infrared, bottom 16 bits from full spectrum
  //to calculate Lux
  lum = tsl.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;

  //lux calculated using TSL2591 library function
  lux = tsl.calculateLux(full, ir);
  //900Lux is a little less than the average overcast daylight Lux value (1075)
  if (lux >= 900)
  {
    daylight = 1;
  }
  else
  {
    daylight = 0;
  }

  // Heat Lamp
  // check if 3pm or before 9pm
  // set heatlamp to 5
  if (now.hour() >= 15 && now.hour() < 21)
  {
    lampLevel = 5;
  }
  else if (now.hour() >= 21)
  {
    if (lampLevel == 5)
    {
      lampLevel = 4;
      dim_track = now + TimeSpan(0, 0, 2, 0);
    }
  }

  // check if 9pm or later
  // check if heat lamp level is 5
  // set lamp level to 4, set dim_track to 2 mins in future
  // check if heat lamp level is 4 and time is equal to dim_track
  // set lamp level to 3, set dim_track to 2 mins in future
  // check if heat lamp level is 3 and time is equal to dim_track
  // set lamp level to 2
  // set dim_track to 2 mins in future
  // check if heat lamp level is 2 and time is equal to dim_track
  // set lamp level to 2
  // set dim_track to 2 mins in future
  // check if heat lamp level is 1 and time is equal to dim_track
  // set lamp level to 0

  // SD (Format: Timestamp, Food(g), Water(mL), Temp (C), Daylight (Y/N))

  if ((now.hour() >= future.hour()) && SD.begin(SD_PIN))
  {
    future = now + TimeSpan(0, 1, 0, 0);

    // Log data to SD card
    myFile = SD.open("test.csv", FILE_WRITE);

    // Writes sensor values
    if (myFile)
    {

      // Timestamp
      myFile.print(now.year());
      myFile.print('/');
      myFile.print(now.month());
      myFile.print('/');
      myFile.print(now.day());
      myFile.print(" (");

      myFile.print(now.hour());
      myFile.print(':');
      myFile.print(now.minute());
      myFile.print(':');
      myFile.print(now.second());
      myFile.print(")");
      myFile.print(",");

      myFile.print(food_grams); // Food
      myFile.print(",");

      myFile.print(water_mL); // Water
      myFile.print(",");

      myFile.print(temperature); // Temperature
      myFile.print(",");

      myFile.print(daylight); // Daylight
      myFile.print(",");

      myFile.println();
      myFile.close();
    }
    else
      Serial.println("Error opening test.csv");
  }

  delay(1000); // delay in between reads for stability
}
