#ifndef DISPLAY_H
#define DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>

#define LCD_BL_PIN 4
#define LCD_BL_PWM_CHANNEL 0

extern TFT_eSPI tft;

class Display
{
    private:



    public:
    void init();
    void routine();
    void setBackLight(float);
};

#endif