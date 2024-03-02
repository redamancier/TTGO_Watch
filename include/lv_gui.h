#ifndef LV_GUI_H
#define LV_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

	//extern lv_img_dsc_t screen_buffer;

extern lv_obj_t * scr_screen_open;

typedef struct
{
    lv_obj_t * scr_user1;
    lv_obj_t * scr_user2;
    lv_obj_t * scr_user3;
    
};



	void lv_screen_on_gui(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  