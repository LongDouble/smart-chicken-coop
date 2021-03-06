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
#include <Adafruit_TSL2591.h>

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

// Heat lamp objects
Lamp lamp(LAMP_PIN);
WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);
int lampLevel;

// DS3231 objects
RTC_DS3231 rtc;
DateTime future, now, dim_track;
float temperature;

//Daylight sensor objects
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
uint32_t lum;
uint16_t ir, full;
float lux;
bool daylight;

//Food/water objects;
float water_mL, food_grams;
float distance_away = 3.1; //custom distance for container to sensor
float radius = 3;          //custom container radius
float height = 13.6;       //custom container height
float density = 0.82844;   //custom density of chicken feed g/mL
float food_em = 63.71;     // 20% of custom maximum food mass (g)
float water_em = 76.91;    // 20% of custom maximum water volume (mL)

//Configuring gain and integration time on daylight sensor
void configureSensor(void)
{
  //setting gain on the TSL2591 to average lighting setting
  tsl.setGain(TSL2591_GAIN_MED); // 25x gain

  //setting duration for Lux integration
  tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
}

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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  future = rtc.now();

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

void loop()
{

  // Timestamp / Temperature
  now = rtc.now();
  temperature = rtc.getTemperature();

  // Food Tracking
  float mL_val, food_distance;
  double food_timeSignal;

  //prepare clean trig pulse with writing low for 5us
  digitalWrite(FOOD_TRIG, LOW);
  delayMicroseconds(5);

  //datasheet requires at least 10us of HIGH pulse
  digitalWrite(FOOD_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(FOOD_TRIG, LOW);

  //read length of HIGH pulse in microseconds
  food_timeSignal = pulseIn(FOOD_ECHO, HIGH);

  //use time to calculate distance in cm (speed of sound ~340m/s = 0.034cm/us, halve time taken to get one length)
  food_distance = food_timeSignal * 0.034 / 2;

  //V = L*pi*(r^2)
  mL_val = (height + distance_away - food_distance) * (3.14159 * radius * radius);
  food_grams = mL_val * density;

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
  // check if 9pm or later
  else if (now.hour() >= 21)
  {
    // if 9pm or later and heat lamp is on level 5
    if (lampLevel == 5)
    {
      lampLevel = 4;

      // set next dim level to two minutes in future
      dim_track = now + TimeSpan(0, 0, 2, 0);
    }
    // if after 9pm, heat lamp is on level 4, and 2 minutes has passed
    else if (lampLevel == 4 && now >= dim_track)
    {
      lampLevel = 3;

      // set next dim level to two minutes in future
      dim_track = now + TimeSpan(0, 0, 2, 0);
    }
    // if after 9pm, heat lamp is on level 3, and 2 minutes has passed
    else if (lampLevel == 3 && now >= dim_track)
    {
      lampLevel = 2;

      // set next dim level to two minutes in future
      dim_track = now + TimeSpan(0, 0, 2, 0);
    }
    // if after 9pm, heat lamp is on level 2, and 2 minutes has passed
    else if (lampLevel == 2 && now >= dim_track)
    {
      lampLevel = 1;

      // set next dim level to two minutes in future
      dim_track = now + TimeSpan(0, 0, 2, 0);
    }
    // if after 9pm, heat lamp is on level 1, and 2 minutes has passed
    else if (lampLevel == 1 && now >= dim_track)
    {
      lampLevel = 0; // turn lamp off
    }
  }

  // set lamp to appropriate level
  lamp.set(lampLevel);

  // check if food or water is too low
  if (water_mL <= water_em || food_grams <= food_em)
  {

    // check if red light should be on
    if (water_mL <= water_em)
    {
      warninglights.removeGreen();
      warninglights.addRed();
    }
    else
    {
      warninglights.removeRed();
    }

    // check if yellow light should be on
    if (food_grams <= food_em)
    {
      //green off, add yellow
      warninglights.removeGreen();
      warninglights.addYellow();
    }
    else
    {
      warninglights.removeYellow();
    }
  }

  // no water or food shortage, turn on green light
  else
  {
    warninglights.green();
  }

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
      myFile.print("/");

      myFile.print(now.hour());
      myFile.print('/');
      myFile.print(now.minute());
      myFile.print('/');
      myFile.print(now.second());

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
