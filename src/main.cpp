
#include "MPU6050_tockn.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Wire.h"
#include "all.h"
#include "button.h"
#include "display.h"
#include "hand.h"
#include "hand_port.h"
#include "hand_user.h"
#include "lvgl.h"
#include "modular.h"
// #include "lv_gui.h"
#include "bmp.h"
// #include "esp_adc_cal.h"
/***Component objects***/
Display screen;
Button btn1(BUTTON_1);
Button btn2(BUTTON_2);
lv_my_ui lv_ui;
MPU6050 mpu6050(Wire);
hand_t hand_structure;

/*some int*/

int btnClick = false;
long timer = 0;
uint8_t hand_feature_signal = PEACE;
uint8_t hand_action_useful = 0;
uint32_t targetTime;
bool HandOnFlag = false;
bool scr_palm = false;
uint32_t scr_on_count = 0;

float pitch, roll, yaw;
int16_t pitch_raw, roll_raw, yaw_raw;
float pitch_filter, roll_filter, yaw_filter;

int16_t gyrox_raw, gyroy_raw, gyroz_raw;
short gyrox, gyroy, gyroz;

short accx, accy, accz;

int16_t accx_raw, accy_raw, accz_raw;
float accx_filter, accy_filter, accz_filter;

uint8_t count_for_hand;

static uint8_t conv2d(const char *p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3),
        ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

/*some basic functions*/
void button_init() {

    btn1.setLongClickHandler([](Button &b) {
        btnClick = false;
        // lv_event_send_refresh(lv_ui.show_label);
        // lv_scr_load(lv_ui.home);
    });
    btn1.setPressedHandler([](Button &b) {
        /*add your event when the button was pressed*/

        btnClick = false;
        // lv_scr_load(lv_ui.scenes);
        HandOnFlag = false;
    });

    btn2.setLongClickHandler([](Button &b) {
        btnClick = false;
    });

    btn2.setPressedHandler([](Button &b) {
        btnClick = false;
        HandOnFlag = true;
        hand_init_again();
        lv_label_set_text(lv_ui.show_label, "#ff0000 PEACE#");
        lv_scr_load(lv_ui.home);
    });
}

void button_loop() {
    btn1.loop();
    btn2.loop();
}

void espDelay(int ms) {
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
}

void setup() {
    /*Bluetooth init in setup*/
    Serial.begin(115200); /* prepare for possible serial debug */
    // tft.setRotation(1);

    // tft.setRotation(3);

    /***Init screen ***/
    screen.init();
    espDelay(500);

    /***Init button***/

    /*init IMU*/
    Wire.begin();
    mpu6050.begin();

    /*hand init*/
    hand_init(&hand_structure);

    /*init mudular as know as GUI objects initialize*/
    setup_ui(&lv_ui);

    tft.setRotation(1);
    screen.setBackLight(0.2);
    tft.pushImage(0, 0, 240, 135, ttgo);
    // lv_screen_on_gui();
    espDelay(5000);
    tft.setRotation(0);

    button_init();
    mpu6050.calcGyroOffsets(true);
    // lv_scr_load(lv_ui.scenes);
    // lv_ui.scenes = lv_scr_act();
}

void loop() {

    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        ss++; // Advance second
        if (ss == 60) {
            ss = 0;
            mm++; // Advance minute
            if (mm > 59) {
                mm = 0;
                hh++; // Advance hour
                if (hh > 23) {
                    hh = 0;
                }
            }
        }
    }

    lv_event_send_refresh(lv_ui.label_second);
    lv_event_send_refresh(lv_ui.label_minute);
    lv_event_send_refresh(lv_ui.label_hour);

    screen.routine();

    if (millis() - timer > 6) {

        pitch = mpu6050.getAngleX();
        roll = mpu6050.getAngleY();
        yaw = mpu6050.getAngleZ();

        gyrox = mpu6050.getGyroX();
        gyroy = mpu6050.getGyroY();
        gyroz = mpu6050.getGyroZ();

        accx = mpu6050.getAccX();
        accy = mpu6050.getAccY();
        accz = mpu6050.getAccZ();

        timer = millis();
    }

    mpu6050.update();

    if (HandOnFlag == true) {
        // hand_init_again();
        hand_handler();
        hand_action_useful = hand_return_action_usefel();
        if (hand_action_useful == 1) {
            hand_feature_signal = hand_return_cur_action();
            lv_event_send_refresh(lv_ui.show_label);
        }
        // count_for_hand++;
    }
    if (scr_palm == true) {
        scr_on_count++;
    }
    if (scr_on_count == 5000) {
        scr_palm = false;
        lv_scr_load(lv_ui.scenes);
        scr_on_count = 0;
    }

    button_loop();
}
