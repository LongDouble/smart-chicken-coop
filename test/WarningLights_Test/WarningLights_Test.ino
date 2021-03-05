#include <WarningLights.h>

#define GREEN_PIN 2
#define YELLOW_PIN 3
#define RED_PIN 4

WarningLights warninglights(GREEN_PIN, YELLOW_PIN, RED_PIN);

void setup()
{
}

void loop()
{
  // Turning on green light...
  warninglights.green();
  delay(5000);

  // Turning on yellow light...
  warninglights.yellow();
  delay(5000);

  // Turning on the red light...
  warninglights.red();
  delay(5000);

  // Turning off all lights...
  warninglights.off();
  delay(5000);
}
