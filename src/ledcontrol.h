#ifndef _LEDCONTROL_H_
#define _LEDCONTROL_H_

// -------------LED ------------
//stop LED flickering when wifi is on: https://github.com/FastLED/FastLED/issues/306
// #define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0 // include BEFORE FastLED.h

#include <FastLED.h>
#include "common.h"


class LedControl
{
public:
    LedControl();
    static void setupLed();
    static void loopLed();
    static void showLEDPattern();
    static void changeLEDPattern();
    static void ledRainbow();
    static void ledRed();
    static void ledNodeCount(int noNodes);
    static CRGB leds[9];
};

// extern LedControl ledControl;


#endif // !_LEDCONTROL_H_
