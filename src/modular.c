#include "modular.h"
#include "lvgl.h"
#include <stdio.h>
#include "all.h"
#include "hand_user.h"
#include "Arduino.h"

// lv_obj_t * scr1 = lv_obj_create(NULL, NULL);
// lv_obj_t * scr2 = lv_obj_create(NULL, NULL);
// lv_obj_t * scr3 = lv_obj_create(NULL, NULL);
// lv_obj_t * scr4 = lv_obj_create(NULL, NULL);
char * show_content;
uint8_t signal_hand1;
feature_class hand_feature;

extern bool HandOnFlag;
extern bool scr_palm;

extern float pitch;
extern float roll;
extern float yaw;

extern short gyrox;
extern short gyroy;
extern short gyroz;

extern short accx;
extern short accy;
extern short accz;

extern int16_t gyrox_raw;
extern int16_t gyroy_raw;
extern int16_t gyroz_raw;

extern int16_t accx_raw;
extern int16_t accy_raw;
extern int16_t accz_raw;

extern uint8_t hh;
extern uint8_t mm;
extern uint8_t ss;

void setup_ui(lv_my_ui* ui)
{
    setup_scr_home(ui);
    lv_scr_load(ui->home);

    setup_scr_scenes(ui);
    lv_scr_load(ui->scenes);

    // setup_scr_show_scene(ui);
    // lv_scr_load(ui->show_scene);
}



void label_event_second_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", (int)(ss));
    }
}

void label_event_minute_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", (int)(mm));
    }
}

void label_event_hour_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", (int)(hh));
    }
}

void label_event_show_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        switch (hand_feature_signal)
        {
        case palm:
            // Serial.println("PALM");
            lv_label_set_text(label, "RAISE");
            HandOnFlag = false;
            break;
        case applause:
            // Serial.println("APPLAUSE");
            lv_label_set_text(label, "APPLAUSE");
            HandOnFlag = false;
            // lv_scr_load(lv_ui.scenes);
            break;
        case put:
            // Serial.println("RAISE");
            lv_label_set_text(label, "PALM");
            scr_palm = true;
            HandOnFlag = false;
        default:
            // hand_signal = false;
            break;
        }
    }
}