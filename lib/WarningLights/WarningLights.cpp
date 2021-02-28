#include "WarningLights.h"

WarningLights::WarningLights(byte greenPin, byte yellowPin, byte redPin)
{
    this->greenPin = greenPin;
    this->yellowPin = yellowPin;
    this->redPin = redPin;

    init();
}

void WarningLights::init()
{
    pinMode(greenPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    green();
}

void WarningLights::green()
{
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, HIGH);
}

void WarningLights::yellow()
{
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, HIGH);
}

void WarningLights::red()
{
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, LOW);
}

void WarningLights::off()
{
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, HIGH);
}

void WarningLights::addGreen()
{
    digitalWrite(greenPin, LOW);
}

void WarningLights::addYellow()
{
    digitalWrite(yellowPin, LOW);
}

void WarningLights::addRed()
{
    digitalWrite(redPin, LOW);
}

void WarningLights::removeGreen()
{
    digitalWrite(greenPin, HIGH);
}

void WarningLights::removeYellow()
{
    digitalWrite(yellowPin, HIGH);
}

void WarningLights::removeRed()
{
    digitalWrite(redPin, HIGH);
}