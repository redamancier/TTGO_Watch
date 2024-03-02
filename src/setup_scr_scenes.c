#include "lvgl.h"
#include <stdio.h>
#include "modular.h"


void setup_scr_scenes(lv_my_ui* ui)
{
    ui->scenes = lv_obj_create(NULL, NULL);

    lv_obj_t * label2 = lv_label_create(ui->scenes, NULL);
    lv_label_set_text(label2, "WatchX");
    lv_label_set_recolor(label2, true);
    lv_obj_align(label2, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    /*test : Create a button*/

    static lv_style_t style_shadow;
    lv_style_init(&style_shadow);
    lv_style_set_shadow_width(&style_shadow, LV_STATE_DEFAULT, 8);
    // lv_style_set_shadow_spread(&style_shadow, LV_STATE_DEFAULT, 5);
    lv_style_set_shadow_color(&style_shadow, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    lv_obj_t * label3 = lv_label_create(ui->scenes, NULL);
    // lv_label_set_text(label3, "WatchX");
    lv_label_set_recolor(label3, true);
    lv_obj_set_size(label3, 60, 60);
    lv_obj_align(label3, NULL, LV_ALIGN_CENTER, 0, 0);

    ui->obj_second = lv_obj_create(ui->scenes, NULL);
    // lv_obj_add_style(ui->obj_second, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_align(ui->obj_second, NULL, LV_ALIGN_CENTER, 64, 7);
    lv_obj_set_size(ui->obj_second, 30, 30);
    
    ui->label_second = lv_label_create(ui->obj_second, label3);
    lv_label_set_text(ui->label_second, "0");
    lv_obj_align(ui->label_second, ui->obj_second, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(ui->label_second, label_event_second_cb);


    ui->obj_minute = lv_obj_create(ui->scenes, NULL);
    // lv_obj_add_style(ui->obj_minute, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_set_size(ui->obj_minute, 30, 30);
    lv_obj_align(ui->obj_minute, NULL, LV_ALIGN_CENTER, 0, 0);

    ui->label_minute = lv_label_create(ui->obj_minute, label3);
    lv_label_set_text(ui->label_minute, "0");
    lv_obj_align(ui->label_minute, ui->obj_minute, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(ui->label_minute, label_event_minute_cb);
    
    ui->obj_hour = lv_obj_create(ui->scenes, NULL);
    // lv_obj_add_style(ui->obj_hour, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_set_size(ui->obj_hour, 30, 30);
    lv_obj_align(ui->obj_hour, NULL, LV_ALIGN_CENTER, -36,0);

    ui->label_hour = lv_label_create(ui->obj_hour, label3);
    lv_label_set_text(ui->label_hour, "0");
    lv_obj_align(ui->label_hour, ui->obj_hour, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(ui->label_hour, label_event_hour_cb); 


    static lv_style_t default_style;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

	lv_obj_add_style(ui->scenes, LV_BTN_PART_MAIN, &default_style);

}