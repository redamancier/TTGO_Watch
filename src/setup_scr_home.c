#include "modular.h"
#include "lvgl.h"
#include <stdio.h>


void setup_scr_home(lv_my_ui* ui)
{
	//Write codes home
	ui->home = lv_obj_create(NULL, NULL);

    static lv_style_t show_style_shadow;
    lv_style_init(&show_style_shadow);
    lv_style_set_shadow_width(&show_style_shadow, LV_STATE_DEFAULT, 8);
    lv_style_set_shadow_spread(&show_style_shadow, LV_STATE_DEFAULT, 5);
    lv_style_set_shadow_color(&show_style_shadow, LV_STATE_DEFAULT, LV_COLOR_BLUE);


    static lv_style_t home_style;
	lv_style_init(&home_style);
	lv_style_set_bg_color(&home_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&home_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&home_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&home_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

	lv_obj_add_style(ui->home, LV_BTN_PART_MAIN, &home_style);

    ui->show_obj = lv_obj_create(ui->home, NULL);
    lv_obj_add_style(ui->show_obj, LV_BTN_PART_MAIN, &show_style_shadow);
    lv_obj_set_size(ui->show_obj, 100, 100);
    lv_obj_align(ui->show_obj, NULL, LV_ALIGN_CENTER, 0, 0);

    ui->show_label = lv_label_create(ui->home, NULL);
    lv_label_set_long_mode(ui->show_label, LV_LABEL_LONG_BREAK);
    lv_obj_set_size(ui->show_label, 100, 40);
    lv_label_set_recolor(ui->show_label, true);
    lv_label_set_text(ui->show_label, "#ff0000 PEACE#");
    lv_obj_align(ui->show_label, ui->show_obj, LV_ALIGN_CENTER, 22, 0);
    lv_obj_set_event_cb(ui->show_label, label_event_show_cb);

    ui->show_ing_obj = lv_obj_create(ui->home, NULL);
    lv_obj_add_style(ui->show_ing_obj, LV_BTN_PART_MAIN, &show_style_shadow);
    lv_obj_set_size(ui->show_ing_obj, 20, 20);
    lv_obj_align(ui->show_ing_obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);




}