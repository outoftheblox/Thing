#include "BlinkPattern.h"

using namespace ootb;

BlinkPattern::BlinkPattern(uint8_t _pin)
    : pin(_pin)
{
    pinMode(pin, OUTPUT);
}

BlinkPattern::~BlinkPattern()
{
}

void BlinkPattern::begin()
{
    pinMode(pin, OUTPUT);
}

void BlinkPattern::handle()
{
    if (size == 0 || millis() < nextChange)
        return;
    nextChange += pattern[index] * duration;
    digitalWrite(pin, index % 2);
    index++;
    if (index >= size)
        index = 0;
}
