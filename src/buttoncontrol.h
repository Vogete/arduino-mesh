#ifndef _BUTTONS_H_
#define _BUTTONS_H_

class ButtonControl
{
private:
    void shortButtonPress();
    void longButtonPress();

    void (*shortButtonPressPtr)();
    void (*longButtonPressPtr)();

    long buttonTimer;
    bool buttonActive;
    bool longPressActive;

public:
    long longPressTime;
    uint8_t buttonPin;

    ButtonControl(uint8_t pinNuber);

    void setupButton(uint8_t pinNuber);
    void handdleButtonPress();
    void setShortButtonPressMethod(void (*callbackFunc)());
    void setLongButtonPressMethod(void (*callbackFunc)());
};


// extern ButtonControl buttonControl;


#endif // !_BUTTONS_H_
