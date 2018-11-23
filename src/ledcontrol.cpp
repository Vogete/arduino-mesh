#include "ledcontrol.h"
#define DATA_PIN 4 //pin D2



CRGB LedControl::leds[9];

LedControl::LedControl(){
}

void LedControl::setupLed()
{
    // CRGB leds[Common::NUM_LEDS];
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, Common::NUM_LEDS);
    FastLED.setMaxRefreshRate(200);
}

void LedControl::loopLed()
{
    FastLED.show(Common::scale);
}


void LedControl::showLEDPattern()
{
    switch (Common::ledPattern)
    {
    case 1:
        ledNodeCount(Common::noNodes);
        break;
    case 2:
        ledRainbow();
        break;
    case 3:
        ledRed();
    default:
        ledNodeCount(Common::noNodes);
        break;
    }
}

void LedControl::changeLEDPattern()
{
    if (Common::ledPattern == 1)
    {
        Common::ledPattern = 2;
    }
    else
    {
        Common::ledPattern = 1;
    }
}

void LedControl::ledRainbow()
{
    for (int i = 0; i < Common::NUM_LEDS; i++)
    {
        uint8_t hue = round((255 / Common::NUM_LEDS) * i);
        leds[i] = CHSV(hue, 255, 255);
        // Serial.println(hue);
        yield();
    }
}


void LedControl::ledRed()
{
    for (int i = 0; i < Common::NUM_LEDS; i++){
        leds[i] = CRGB(255, 0, 0);
        yield();
    }
}

void LedControl::ledNodeCount(int noNodes)
{
    // CRGB leds[Common::NUM_LEDS];

    // int noNodes = meshNodes.size() + 1;
    for (int i = 0; i < Common::NUM_LEDS; i++)
    {
        if (i <= noNodes - 1)
        {
            leds[i] = CRGB(0, 179, 60);
        }
        else
        {
            leds[i].fadeToBlackBy(100);
        }
        yield();
    }
}

// LedControl ledcontrol = LedControl();
