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
    void addGreen();
    void addYellow();
    void addRed();
    void removeGreen();
    void removeYellow();
    void removeRed();
};

#endif