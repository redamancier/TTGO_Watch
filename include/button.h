
#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

#define DEBOUNCE_MS 50
#define LONGCLICK_MS 250
#define DOUBLECLICK_MS 400

#define SINGLE_CLICK   1
#define DOUBLE_CLICK   2
#define TRIPLE_CLICK   3
#define LONG_CLICK     4

/*define the button pins in the follow*/

#define BUTTON_1    35
#define BUTTON_2    0


class Button 
{
    private:
    byte pin;
    int prev_state;
    int state = HIGH;
    byte click_count = 0;
    unsigned int last_click_type = 0;
    unsigned long click_ms;
    unsigned long down_ms;
    unsigned int debounce_time_ms;
    unsigned int down_time_ms = 0;
    bool pressed_triggered = false;
    bool longclick_detected = false;

    typedef void (*CallbackFunction) (Button&);

    CallbackFunction pressed_cb = NULL;
    CallbackFunction released_cb = NULL;
    CallbackFunction change_cb = NULL;
    CallbackFunction tap_cb = NULL;
    CallbackFunction click_cb = NULL;
    CallbackFunction long_cb = NULL;
    CallbackFunction double_cb = NULL;
    CallbackFunction triple_cb = NULL;

    public:
    Button(){pin = -1;}
    Button(byte attachTo, byte buttonMode = INPUT_PULLUP, unsigned int debounceTimeout = DEBOUNCE_MS);
    void setDebounceTime(unsigned int ms);

    void setChangedHandler(CallbackFunction f);
    void setPressedHandler(CallbackFunction f);
    void setReleasedHandler(CallbackFunction f);
    void setClickHandler(CallbackFunction f);
    void setTapHandler(CallbackFunction f);
    void setLongClickHandler(CallbackFunction f);
    void setDoubleClickHandler(CallbackFunction f);
    void setTripleClickHandler(CallbackFunction f);

    unsigned int wasPressedFor();
    boolean isPressed();

    unsigned int getNumberOfClicks();
    unsigned int getClickType();
    uint8_t getAttachPin(){return pin;}
    bool operator==(Button &rhs);

    void loop();


};






















#endif
