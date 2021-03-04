#ifndef LAMP_H
#define LAMP_H

#include <Arduino.h>

class Lamp
{

private:
    byte pin;
    int pwm[6] = {0, 80, 110, 140, 180, 255};

public:
    Lamp(byte pin);

    void init();

    void set(int = 0);
};

#endif