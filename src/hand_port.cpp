#include "hand.h"
#include "all.h"
#include "modular.h"
#include "display.h"
#include "lvgl.h"
#include "TFT_eSPI.h"
/*user include*/

extern lv_my_ui lv_ui;


#define HAND_RAISE_SIGNAL  0
#define HAND_PEACE_SIGNAL  1

bool hand_signal = false;

/*end of uset include*/

/*angle*/
float get_imu_pitch()
{

    /*return float*/
    return pitch;
}

float get_imu_roll()
{

    /*return float*/
    return roll;
}

float get_imu_yaw()
{

    /*return float*/
    return yaw;
}
/*end of angle*/

/*gyro*/
short get_imu_gyrox() // get gyrox
{

    /*return short*/
    return gyrox;
}

short get_imu_gyroy() // get gyroy
{

    /*return short*/
    return gyroy;
}

short get_imu_gyroz() // get gyroz
{

    /*return short*/
    return gyroz;
}
/*end of gyro*/

/*acc*/
short get_imu_accx() // get accx 
{

    /*return short*/
    return accx;
}

short get_imu_accy() // get accy 
{

    /*return short*/
    return accy;
}

short get_imu_accz() // get accz
{

    /*return short*/
    return accz;
}
/*end of acc*/

/*led*/
void led_on()
{
    static lv_style_t led_on_style;
    lv_style_init(&led_on_style);
    lv_style_set_bg_color(&led_on_style, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_add_style(lv_ui.show_ing_obj, LV_BTN_PART_MAIN, &led_on_style);
}

void led_off()
{
    static lv_style_t led_off_style;
    lv_style_init(&led_off_style);
    lv_style_set_bg_color(&led_off_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(lv_ui.show_ing_obj, LV_BTN_PART_MAIN, &led_off_style);
}
/*end of led*/

// int main()
// {

// }