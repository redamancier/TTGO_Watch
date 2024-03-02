
#ifndef MODULAR_H
#define MODULAR_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "lvgl.h"

extern uint32_t targetTime;
extern uint8_t hh;
extern uint8_t mm;
extern uint8_t ss;

extern char * show_content;
extern uint8_t signal_hand1;

typedef struct 
{
    lv_obj_t * show_obj;
    lv_obj_t * show_label;
    lv_obj_t * show_ing_obj;
    lv_obj_t * obj_second;
    lv_obj_t * label_second;
    lv_obj_t * obj_minute;
    lv_obj_t * label_minute;
    lv_obj_t * obj_hour;
    lv_obj_t * label_hour;
    lv_obj_t * home;
    lv_obj_t * home_cpicker0;
    lv_obj_t * scenes;
    lv_obj_t * scenes_canvas;

}lv_my_ui;


extern lv_my_ui lv_ui;

void setup_ui(lv_my_ui* ui);
void setup_scr_home(lv_my_ui* ui);
void setup_scr_scenes(lv_my_ui* ui);
void setup_scr_show_scene(lv_my_ui* ui);

///////////////////API for show//////////////////////

/////////////////////////////////////////////////////
void label_event_second_cb(lv_obj_t * label, lv_event_t event);
void label_event_minute_cb(lv_obj_t * label, lv_event_t event);
void label_event_hour_cb(lv_obj_t * label, lv_event_t event);
void label_event_show_cb(lv_obj_t * label, lv_event_t event);
#ifdef __cplusplus
}
#endif
#endif