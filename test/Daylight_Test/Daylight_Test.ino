//Wire.h allows for I2C communications on Arduino
#include <Wire.h>

//Including TSL2591 functionality libraries
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

//declare sensor object for TSL2591 library
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

//TSL2591 utilizes a full spectrum sensor and an infrared sensor. To use both of these values simultaneously for 
//calculating Lux, 32-bit value is declared for total luminosity and two 16-bit values for IR and full spectrum
uint32_t lum;
uint16_t ir, full;

//variable to store calculated Lux
float lux;

//variable for daylight hours
int daylight;

//Configuring gain and integration time on sensor
void configureSensor(void)
{
  //setting gain on the TSL2591 to default medium setting, which is recommended for average lighting conditions
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  
  //setting duration for which Lux integration is performed, provided by TSL2591 library
   tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS); //second to highest integration time for 
                                                 //increased accuracy (but not extreme conditions)
}

float get_lux(){
  float lux_val;
  
  //Read 32 bits from both sensors in TSL2591, top 16 bits from infrared, bottom 16 bits from full spectrum
  //to calculate Lux
  lum = tsl.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;

  //lux calculated using TSL2591 library function
  lux_val = tsl.calculateLux(full, ir);

  //For testing purposes, including Lux reading
  //Serial.print(F("Lux: ")); 
  //Serial.print(lux_val);

  return lux_val;
}

int is_daylight(){
  //declare daylight value in scope
  int daylight_val;
  
  //If the Lux rating qualifies as daylight, program returns 1. Otherwise, 0 is returned for hourly daylight status
  //Serial.print("\t Daylight: ");

  //900Lux is a little less than the average overcast daylight Lux value (1075), the "minimum" expected case
  if(lux >= 900){
    daylight_val = 1;
    //Serial.println(1);
  }

  else{
    daylight_val = 0;
    //Serial.println(0);
  }

  return daylight_val;
}

void setup() {
  //initialize serial communications
  Serial.begin(9600);
}

void loop() {
  lux = get_lux();
  daylight = is_daylight();
  //for sake of demo, delay of 2 seconds
  //delay(2000);
}
