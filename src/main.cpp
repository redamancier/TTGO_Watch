#include "lvgl.h"
#include "TFT_eSPI.h"
#include "SPI.h"
#include "display.h"
#include "button.h"
// #include "lv_examples/lv_examples.h"

/***Component objects***/

Display screen;
Button btn1(BUTTON_1);
Button btn2(BUTTON_2);

/*some int */

int vref = 1100;
int btnClick = false;


lv_obj_t * scr1 = lv_obj_create(NULL, NULL);
lv_obj_t * scr2 = lv_obj_create(NULL, NULL);
lv_obj_t * scr3 = lv_obj_create(NULL, NULL);
lv_obj_t * scr4 = lv_obj_create(NULL, NULL);



float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx=64, osy=64, omx=64, omy=64, ohx=64, ohy=64;  // Saved H, M, S x & y coords
uint16_t x0=0, x1=0, yy0=0, yy1=0;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

boolean initial = 1;

lv_obj_t * obj_second = NULL;
lv_obj_t * label_second = NULL;
lv_obj_t * obj_minute = NULL;
lv_obj_t * label_minute = NULL;
lv_obj_t * obj_hour = NULL;
lv_obj_t * label_hour = NULL;

/*here is some functions of events*/
void label_event_second_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", ss);
    }
}

void label_event_minute_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", mm);
    }
}

void label_event_hour_cb(lv_obj_t * label, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text_fmt(label, "%d", hh);
    }
}

/*some basic functions*/
void button_init()
{
    btn1.setLongClickHandler([](Button & b)
    {
        btnClick = false;
        int r = digitalRead(TFT_BL);
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Press again to wake up",  tft.width() / 2, tft.height() / 2 );
        delay(6000);
        digitalWrite(TFT_BL, !r);

        tft.writecommand(TFT_DISPOFF);
        tft.writecommand(TFT_SLPIN);
        //After using light sleep, you need to disable timer wake, because here use external IO port to wake up
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
        // esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
        delay(200);
        esp_deep_sleep_start();
    });
    btn1.setPressedHandler([](Button & b) 
    {
        /*add your event when the button was pressed*/


        btnClick = true;
    });

    btn2.setPressedHandler([](Button & b)
    {
        btnClick = false;
        /*add your event when the button2 was pressed*/
    });
}

void button_loop()
{
    btn1.loop();
    btn2.loop();
}





void setup()
{
	Serial.begin(115200); /* prepare for possible serial debug */
    
    /***Init screen ***/
    screen.init();
    screen.setBackLight(0.2);
    scr1 = lv_scr_act();

    /***Init button***/
    button_init();




    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label2, "Create By Alex");
    lv_label_set_recolor(label2, true);
    lv_obj_align(label2, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    /*test : Create a button*/

    static lv_style_t style_shadow;
    lv_style_init(&style_shadow);
    lv_style_set_shadow_width(&style_shadow, LV_STATE_DEFAULT, 8);
    // lv_style_set_shadow_spread(&style_shadow, LV_STATE_DEFAULT, 5);
    lv_style_set_shadow_color(&style_shadow, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    lv_obj_t * label3 = lv_label_create(lv_scr_act(), NULL);
    // lv_label_set_text(label3, "WatchX");
    lv_label_set_recolor(label3, true);
    lv_obj_align(label3, NULL, LV_ALIGN_CENTER, 0, 0);

    targetTime = millis() + 1000; 

    obj_second = lv_obj_create(lv_scr_act(), NULL);
    // lv_obj_add_style(obj_second, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_align(obj_second, NULL, LV_ALIGN_IN_RIGHT_MID, 39, 0);
    lv_obj_set_size(obj_second, 40, 40);
    
    label_second = lv_label_create(obj_second, label3);
    lv_label_set_text(label_second, "0");
    lv_obj_align(label_second, obj_second, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(label_second, label_event_second_cb);


    obj_minute = lv_obj_create(lv_scr_act(), NULL);
    // lv_obj_add_style(obj_minute, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_set_size(obj_minute, 40, 40);
    lv_obj_align(obj_minute, NULL, LV_ALIGN_CENTER, 0, 0);

    label_minute = lv_label_create(obj_minute, label3);
    lv_label_set_text(label_minute, "0");
    lv_obj_align(label_minute, obj_minute, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(label_minute, label_event_minute_cb);
    
 
    obj_hour = lv_obj_create(lv_scr_act(), NULL);
    // lv_obj_add_style(obj_hour, LV_OBJ_PART_MAIN, &style_shadow);
    lv_obj_set_size(obj_hour, 40, 40);
    lv_obj_align(obj_hour, NULL, LV_ALIGN_IN_LEFT_MID, 2, 0);

    label_hour = lv_label_create(obj_hour, label3);
    lv_label_set_text(label_hour, "0");
    lv_obj_align(label_hour, obj_hour, LV_ALIGN_CENTER, -3, 0);
    lv_obj_set_event_cb(label_hour, label_event_hour_cb); 


    static lv_style_t default_style;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

	lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);

	scr2 = lv_scr_act();


}


void loop()
{
    if (targetTime < millis()) {
        targetTime = millis()+1000;
        ss++;              // Advance second
        if (ss==60) {
         ss=0;
         mm++;            // Advance minute
        if(mm>59) {
            mm=0;
            hh++;          // Advance hour
            if (hh>23) {
            hh=0;
                }
            }
        }
    }

    screen.routine();

    lv_event_send_refresh(label_second);
    lv_event_send_refresh(label_minute);
    lv_event_send_refresh(label_hour);

    button_loop();
}


