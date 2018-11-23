#include "buttoncontrol.h"
#include "common.h"
#include "ledcontrol.h"
#include "meshcontrol.h"

// #define BUTTON_PIN 15 //D8

uint8_t buttonPin;

long longPressTime;
long buttonTimer;
bool buttonActive;
bool longPressActive;

void (*shortButtonPressPtr)();
void (*longButtonPressPtr)();

ButtonControl::ButtonControl(uint8_t pinNumber){
    buttonPin = pinNumber;
    setupButton(buttonPin);

    buttonTimer = 0;
    longPressTime = 1000;
    buttonActive = false;
    longPressActive = false;

    shortButtonPressPtr = NULL;
    longButtonPressPtr = NULL;
}

void ButtonControl::setupButton(uint8_t pinNumber)
{
    pinMode(pinNumber, INPUT_PULLUP);
}

void ButtonControl::handdleButtonPress()
{
    // TODO: button debounce
    Common::buttonState = digitalRead(buttonPin);

	if (Common::buttonState == LOW) {
        if (buttonActive == false) {
			buttonActive = true;
			buttonTimer = millis();
		}

		if ( (millis() - buttonTimer > longPressTime) && (longPressActive == false) ) {
			longPressActive = true;
            Serial.println("Long button press detected");
            longButtonPress();
		}
	} else {
		if (buttonActive == true) {
			if (longPressActive == true) {
				longPressActive = false;
			} else {
                Serial.println("Short button press detected");
                shortButtonPress();
			}
			buttonActive = false;
		}
	}

}

void ButtonControl::shortButtonPress() {

    if (shortButtonPressPtr != NULL) {
        (*shortButtonPressPtr)();
    }
    else
    {
        Serial.println("POINTER NULL (short press)");
    }

}

void ButtonControl::longButtonPress() {
    if (longButtonPressPtr != NULL) {
        (*longButtonPressPtr)();
    }
    else
    {
        Serial.println("POINTER NULL (long press)");
    }
}

void ButtonControl::setShortButtonPressMethod(void (*callbackFunc)()){

    shortButtonPressPtr = *callbackFunc;
}

void ButtonControl::setLongButtonPressMethod(void (*callbackFunc)()){
    longButtonPressPtr = *callbackFunc;
}
