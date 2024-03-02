#ifndef __HAND_H
#define __HAND_H
#include <stdint.h>
/*
定义成员：
当前状态（由状态机可得一共有四种状态机状态组成：空闲IDLE 、采集COLLECT、等待退出WAIT_EXIT、退出（分析数据）EXIT_AND_ANA）

初始角度簇
末态角度簇

队列指针
队列长度
队列能量总和
队列均值能量

角度队列
角速度队列
加速度差分队列


*/

// #ifndef uint8_t 
// #define uint8_t unsigned char
// #endif

// #ifndef uint32_t
// #define uint32_t unsigned int
// #endif

/*gesture*/
    /*put*/
#define PUT_ANGLE_END_0     end_roll
#define PUT_ANGLE_START_0   start_roll

#define PUT_ANGLE_END_1     end_yaw
#define PUT_ANGLE_START_1   start_yaw
    /*end of put*/

    /*applause*/
#define APPLAUSE_ANGLE_START    start_roll
#define APPLAUSE_ANGLE_END      end_roll
    /*end of applause*/

    /*palm*/
#define PALM_ANGLE_END          end_pitch
    /*end of palm*/
/*end of gesture*/

#define HAND_GYRO_PEAK_MIN 3000
#define HAND_ACCE_PEAK_MIN 5000
#define HAND_ANGLE_PEAK_MIN 0

#define HAND_START_GYRO_MIN 1000
#define HAND_START_ACC_MIN 	25000

#define HAND_QUEUE_LENGTH 500
#define HAND_QUEUE_MIN_LENGTH 50
#define HAND_QUEUE_MIN_ERR 20

typedef enum
{
	IDLE = 0,
	START = 1,
	COLLECT = 2,
	WAIT_EXIT = 3,
	EXIT_AND_ANA = 4	
}statement;

typedef struct
{
	statement cstate;
	uint32_t hand_ccr;
	float start_pitch;
	float start_yaw;
	float start_roll;
	
	float end_pitch;
	float end_yaw;
	float end_roll;
	
	uint32_t queue_pointer;
	uint32_t queue_length;
	float all_power;
	float mean_power;
	
	float queue_pitch[HAND_QUEUE_LENGTH];
	float queue_yaw[HAND_QUEUE_LENGTH];
	float queue_roll[HAND_QUEUE_LENGTH];
	
	short queue_gyrox[HAND_QUEUE_LENGTH];
	short queue_gyroy[HAND_QUEUE_LENGTH];
	short queue_gyroz[HAND_QUEUE_LENGTH];
	
	short queue_ax[HAND_QUEUE_LENGTH];
	short queue_ay[HAND_QUEUE_LENGTH];
	short queue_az[HAND_QUEUE_LENGTH];
	
	short queue_ax_min[HAND_QUEUE_MIN_LENGTH + 1];
	short queue_ay_min[HAND_QUEUE_MIN_LENGTH + 1];
	short queue_az_min[HAND_QUEUE_MIN_LENGTH + 1];
	
	short queue_gyrox_min[HAND_QUEUE_MIN_LENGTH + 1];
	short queue_gyroy_min[HAND_QUEUE_MIN_LENGTH + 1];
	short queue_gyroz_min[HAND_QUEUE_MIN_LENGTH + 1];
	
	float queue_yaw_min[HAND_QUEUE_MIN_LENGTH + 1];
	float queue_pitch_min[HAND_QUEUE_MIN_LENGTH + 1];
	float queue_roll_min[HAND_QUEUE_MIN_LENGTH + 1];
	
    uint8_t debug;
    uint8_t finish_mark;
}hand_t;

typedef enum
{
	x_y=0,
	x_z=1,
	y_z=2
}gravity_class;

typedef enum
{
	none = 0,
	palm = 1,
	applause = 2,
	put = 3
}feature_class;

typedef struct
{
	uint32_t gyrox_wave_max;
	uint32_t gyroy_wave_max;
	uint32_t gyroz_wave_max;
	
	uint32_t accx_wave_max;
	uint32_t accy_wave_max;
	uint32_t accz_wave_max;
	
	uint32_t gyrox_wave_peak_cnt;
	uint32_t gyroy_wave_peak_cnt;
	uint32_t gyroz_wave_peak_cnt;
	
	uint32_t accx_wave_peak_cnt;
	uint32_t accy_wave_peak_cnt;
	uint32_t accz_wave_peak_cnt;
	
	gravity_class start_gravity;
	feature_class action;
	gravity_class gravity;
}feature_t;

#define PALM_MIN_MEAN_POWER 200
#define PALM_MAX_MEAN_PWER 	400
#define PALM_MIN_PEAK 		2000
#define PALM_PEAK_CNT 		1


#endif
