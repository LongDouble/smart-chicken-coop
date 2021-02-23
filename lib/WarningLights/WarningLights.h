#ifndef WARNINGLIGHTS_H
#define WARNINGLIGHTS_H

#include <Arduino.h>

class WarningLights
{

private:
    byte greenPin, yellowPin, redPin;

public:
    WarningLights(byte greenPin, byte yellowPin, byte redPin);

    void init();

    void green();
    void yellow();
    void red();
    void off();
};

#endif