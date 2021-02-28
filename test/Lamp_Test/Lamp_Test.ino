#include <Lamp.h>

#define LAMP_PIN 5

Lamp lamp(LAMP_PIN);

void setup()
{
}

void loop()
{
  // Gently increase lamp brightness
  // every two seconds
  for (int i = 0; i < 6; i++)
  {
    lamp.set(i);
    delay(2000);
  }
}
