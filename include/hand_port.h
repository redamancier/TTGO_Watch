#ifndef __HAND_PORT_H
#define __HAND_PORT_H
#include "hand_port.h"

extern bool hand_signal;

float get_imu_pitch();
float get_imu_roll();
float get_imu_yaw();

short get_imu_gyrox();
short get_imu_gyroy();
short get_imu_gyroz();

short get_imu_accx();
short get_imu_accy();
short get_imu_accz();

void led_on();
void led_off();

#endif