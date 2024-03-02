
#ifndef ALL_H
#define ALL_H

#define PEACE     0
#define PALM      1
#define APPLAUSE  2
#define RAISE     3
#include <stdint.h>

extern bool hand_signal;
extern uint8_t hand_feature_signal;

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



#endif