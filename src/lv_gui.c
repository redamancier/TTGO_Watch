#include "stdlib.h"
#include "undingable.h"
#include "lv_gui.h"

lv_obj_t * scr_screen_open;

void lv_screen_on_gui(void)
{
    scr_screen_open = lv_scr_act();
    lv_obj_t * img_1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img_1, &undingable);
    lv_obj_align(img_1, NULL, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_size(img_1, 240, 135);
}