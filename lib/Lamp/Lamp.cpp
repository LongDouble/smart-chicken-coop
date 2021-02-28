#include "Lamp.h"

Lamp::Lamp(byte pin)
{
    this->pin = pin;
    init();
}

void Lamp::init()
{
    pinMode(pin, OUTPUT);
    set();
}

void Lamp::set(int brightness)
{
    if (brightness > 5)
        brightness = 5;

    analogWrite(pin, pwm[brightness]);
}