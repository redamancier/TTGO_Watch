#include "hand_port.h"
#include "hand.h"
#include "hand_user.h"

void hand_handler(void);
uint32_t hand_cur_power_calc(void);
void hand_feature_data_init(void);
void hand_gestures_ana(void);
void hand_wave_ana(void);
void hand_gravity_change(void);
void hand_feature_data_init(void);
uint32_t hand_cur_power_calc(void);
void hand_state_deal(void);
void hand_state_jump(void);
void hand_init_again(void);

hand_t * hand_pointer = 0;

feature_t feature = {0}; 

/*hand_user*/
unsigned char hand_return_action_usefel()
{
    return hand_pointer -> finish_mark;
}
feature_class hand_return_cur_action()
{
    return feature.action;
}

gravity_class hand_return_cur_gravity()
{
    return feature.gravity;
}
/*end of hand_user*/

/*hand_handler*/
void hand_start_again()
{
    hand_init_again();
    // hand_handler();
}

void hand_handler()
{
//	if(hand_pointer -> debug == 0)
	{
		hand_state_deal();
		hand_state_jump(); //两段式状态机处理
	}
}
/*end of hand_handler*/

/*hand_data_init*/
void hand_queue_min_init()
{
	hand_pointer -> hand_ccr = 0;
	for(int i=0;i<HAND_QUEUE_MIN_LENGTH;i++)
	{
		hand_pointer -> queue_ax_min[i] = 0;
		hand_pointer -> queue_az_min[i] = 0;
		hand_pointer -> queue_ay_min[i] = 0;
	}
}

void hand_queue_init()
{
	for(int i=0; i<HAND_QUEUE_LENGTH; i++)
	{
		hand_pointer -> queue_pitch[i] = 0;
		hand_pointer -> queue_yaw[i] = 0;
		hand_pointer -> queue_roll[i] = 0;
		
		hand_pointer -> queue_gyrox[i] = 0;
		hand_pointer -> queue_gyroy[i] = 0;
		hand_pointer -> queue_gyroz[i] = 0;
		
		hand_pointer -> queue_ax[i] = 0;
		hand_pointer -> queue_ay[i] = 0;
		hand_pointer -> queue_az[i] = 0;
	}
}

void hand_init_again()
{
    hand_pointer -> cstate = IDLE;
	hand_pointer -> start_pitch = 0;
	hand_pointer -> start_yaw = 0;
	hand_pointer -> start_roll = 0;
	
	hand_pointer -> end_pitch = 0;
	hand_pointer -> end_yaw = 0;
	hand_pointer -> end_roll =0;
	
	hand_pointer -> queue_pointer =0;
	hand_pointer -> queue_length =0;
	
	hand_pointer -> all_power = 0;
	hand_pointer -> mean_power = 0;
	
	hand_pointer -> debug = 0;

    hand_pointer -> finish_mark = 0;
	
	hand_feature_data_init();
	hand_queue_init();
	hand_queue_min_init();
}

void hand_init(hand_t* hand)
{
    hand_pointer = hand;

	hand_pointer -> cstate = IDLE;
	hand_pointer -> start_pitch = 0;
	hand_pointer -> start_yaw = 0;
	hand_pointer -> start_roll = 0;
	
	hand_pointer -> end_pitch = 0;
	hand_pointer -> end_yaw = 0;
	hand_pointer -> end_roll =0;
	
	hand_pointer -> queue_pointer =0;
	hand_pointer -> queue_length =0;
	
	hand_pointer -> all_power = 0;
	hand_pointer -> mean_power = 0;
	
	hand_pointer -> debug = 0;

    hand_pointer -> finish_mark = 0;
	
	hand_pointer = hand;
	
	hand_feature_data_init();
	hand_queue_init();
	hand_queue_min_init();
}

void hand_feature_data_init()
{
	feature.gyrox_wave_max = 0;
	feature.gyroy_wave_max = 0;
	feature.gyroz_wave_max = 0;
	
	feature.accx_wave_max = 0;
	feature.accy_wave_max = 0;
	feature.accz_wave_max = 0;
	
	feature.gyrox_wave_peak_cnt = 0;
	feature.gyroy_wave_peak_cnt = 0;
	feature.gyroz_wave_peak_cnt = 0;
	
	feature.accx_wave_peak_cnt = 0;
	feature.accy_wave_peak_cnt = 0;
	feature.accz_wave_peak_cnt = 0;
}

void hand_feature_gravity_init()
{
	feature.start_gravity = x_z;
	feature.action = none;
	feature.gravity = x_z;
}

/*end of hand_data_init*/

/*hand_abs*/
uint32_t hand_int_abs(int num)
{
	if(num>0)
		return num;
	else 
		return -num;
}

float hand_q31_abs(float num)
{
	if(num >= -0.01f)
		return num;
	else 
		return -num;
}
/*end of hand_abs*/

/*hand_imu_data_get*/
void hand_get_imu_data()
{
	hand_pointer -> queue_pitch[hand_pointer -> queue_pointer] = get_imu_pitch();
	hand_pointer -> queue_yaw[hand_pointer -> queue_pointer] = get_imu_yaw();
	hand_pointer -> queue_roll[hand_pointer -> queue_pointer] = get_imu_roll();
	
	hand_pointer -> queue_gyrox[hand_pointer -> queue_pointer] = get_imu_gyrox();
	hand_pointer -> queue_gyroy[hand_pointer -> queue_pointer] = get_imu_gyroy();
	hand_pointer -> queue_gyroz[hand_pointer -> queue_pointer] = get_imu_gyroz();
	
	hand_pointer -> queue_ax[hand_pointer -> queue_pointer] = get_imu_accx();
	hand_pointer -> queue_ay[hand_pointer -> queue_pointer] = get_imu_accy();
	hand_pointer -> queue_az[hand_pointer -> queue_pointer] = get_imu_accz();
}
/*end of hand_imu_data_get*/


/*hand_start_mark*/
uint32_t hand_cur_power_calc()
{
	uint32_t cur_gyro_power = hand_q31_abs(get_imu_gyrox()) + hand_q31_abs(get_imu_gyroy()) + hand_q31_abs(get_imu_gyroz()) ;

	if(cur_gyro_power >= HAND_START_GYRO_MIN)
		return 1;
	else 
		return 0;
}
/*end of hand_start_mark*/


/*hand_state_fun*/
int hand_wati_exit_handler()
{
	static uint32_t la_gyro = 0;

	uint32_t min_power = 0;
	uint32_t i = hand_pointer -> hand_ccr;

    hand_pointer -> queue_ax_min[i] = get_imu_accx();
    hand_pointer -> queue_ay_min[i] = get_imu_accy();
    hand_pointer -> queue_az_min[i] = get_imu_accz();
    
    hand_pointer -> queue_gyrox_min[i] = get_imu_gyrox();
    hand_pointer -> queue_gyroy_min[i] = get_imu_gyroy();
    hand_pointer -> queue_gyroz_min[i] = get_imu_gyroz();
    
    hand_pointer -> queue_yaw_min[i] = get_imu_yaw();
    hand_pointer -> queue_pitch_min[i] = get_imu_pitch();
    hand_pointer -> queue_roll_min[i] = get_imu_roll();

    min_power = hand_int_abs(hand_pointer -> queue_gyrox_min[i])+
                hand_int_abs(hand_pointer -> queue_gyroy_min[i])+
                hand_int_abs(hand_pointer -> queue_gyroz_min[i]);
                                                                
    if(min_power  < HAND_START_GYRO_MIN)
        la_gyro++;
    
    if(la_gyro < HAND_QUEUE_MIN_ERR)
    {
        if(hand_pointer -> hand_ccr < HAND_QUEUE_MIN_LENGTH)
            hand_pointer -> hand_ccr ++;
    }

	if(hand_pointer -> hand_ccr == HAND_QUEUE_MIN_LENGTH)
	{
		for(int i=0;i<HAND_QUEUE_MIN_LENGTH;i++)
		{
			hand_pointer -> queue_ax[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_ax_min[i];
			hand_pointer -> queue_ay[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_ay_min[i];
			hand_pointer -> queue_az[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_az_min[i];
			
			hand_pointer -> queue_gyrox[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_gyrox_min[i];
			hand_pointer -> queue_gyroy[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_gyroy_min[i];
			hand_pointer -> queue_gyroz[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_gyroz_min[i];
			
			hand_pointer -> queue_yaw[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_yaw_min[i];
			hand_pointer -> queue_pitch[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_pitch_min[i];
			hand_pointer -> queue_roll[hand_pointer -> queue_pointer + i] = hand_pointer -> queue_roll_min[i];
		}
		hand_pointer -> queue_pointer += HAND_QUEUE_MIN_LENGTH;
		
		la_gyro = 0;
		return 1;
	}
	
	if(la_gyro < HAND_QUEUE_MIN_ERR)
	{
		return 1;
	}
	else
	{
		la_gyro = 0;
		return 0;
    }
}


void hand_collect_handler()
{
	if(hand_pointer -> queue_pointer < HAND_QUEUE_LENGTH)
	{
		hand_get_imu_data();
		
		hand_pointer -> queue_pointer++;
	}
}


void hand_start_handler()
{
	hand_pointer -> queue_pointer = 0;
	hand_pointer -> queue_length = 0;
	hand_pointer -> all_power = 0;
	hand_pointer -> mean_power =0;
	
	hand_pointer -> end_pitch = 0;
	hand_pointer -> end_roll =0;
	hand_pointer -> end_yaw = 0;
	
	hand_pointer -> start_pitch = hand_pointer -> queue_pitch[0];
	hand_pointer -> start_roll = hand_pointer -> queue_roll[0];
	hand_pointer -> start_yaw = hand_pointer -> queue_yaw[0];
	
	hand_queue_init();
	
}

void hand_exit_ana_handler()
{
	hand_pointer -> end_pitch = hand_pointer -> queue_pitch[hand_pointer -> queue_pointer - 2];
	hand_pointer -> end_yaw = hand_pointer -> queue_yaw[hand_pointer -> queue_pointer  - 2];
	hand_pointer -> end_roll = hand_pointer -> queue_roll[hand_pointer -> queue_pointer  - 2];

	hand_pointer ->all_power = 0;

	for(int i=0;i<hand_pointer -> queue_pointer;i++)
	{
		hand_pointer ->all_power += hand_int_abs(hand_pointer -> queue_gyrox[i]) +
									hand_int_abs(hand_pointer -> queue_gyroy[i]) +
									hand_int_abs(hand_pointer -> queue_gyroz[i]);
	}
	hand_pointer -> mean_power = hand_pointer ->all_power / (hand_pointer -> queue_pointer);
	hand_pointer -> queue_length = hand_pointer -> queue_pointer;
	
	hand_gravity_change();
	hand_wave_ana();
	hand_gestures_ana();
}

/*end of hand_state_fun*/

/*hand_exit_or_wave_ana_fun*/
void hand_gravity_change()
{
	uint32_t cur_length = ((hand_pointer -> queue_pointer) >> 4) * 15;
	uint32_t start_length = (hand_pointer -> queue_pointer) >> 4;
	hand_t* p = hand_pointer;

	if(hand_int_abs(p -> queue_ax[cur_length]) >= hand_int_abs(p -> queue_ay[cur_length]) && hand_int_abs(p -> queue_ay[cur_length]) >= hand_int_abs(p->queue_az[cur_length]))
	{
		feature.gravity = x_y;
	}		
	else if(hand_int_abs(p -> queue_ax[cur_length]) >= hand_int_abs(p -> queue_az[cur_length]) && hand_int_abs(p -> queue_az[cur_length]) >= hand_int_abs(p -> queue_ay[cur_length]))
	{
		feature.gravity = x_z;
	}
	else if(hand_int_abs(p -> queue_ay[cur_length]) >= hand_int_abs(p -> queue_az[cur_length]) && hand_int_abs(p -> queue_az[cur_length]) >= hand_int_abs(p -> queue_ax[cur_length]))
	{
		feature.gravity = y_z;
	}
	
	if(hand_int_abs(p -> queue_ax[start_length]) >= hand_int_abs(p -> queue_ay[start_length]) && hand_int_abs(p -> queue_ay[start_length]) >= hand_int_abs(p->queue_az[start_length]))
	{
		feature.start_gravity = x_y;
	}		
	else if(hand_int_abs(p -> queue_ax[start_length]) >= hand_int_abs(p -> queue_az[start_length]) && hand_int_abs(p -> queue_az[start_length]) >= hand_int_abs(p -> queue_ay[start_length]))
	{
		feature.start_gravity = x_z;
	}
	else if(hand_int_abs(p -> queue_ay[start_length]) >= hand_int_abs(p -> queue_az[start_length]) && hand_int_abs(p -> queue_az[start_length]) >= hand_int_abs(p -> queue_ax[start_length]))
	{
		feature.start_gravity = y_z;
	}
}

void hand_wave_ana(void)
{
	hand_t * p = hand_pointer;
	
	uint8_t gyrox_wave_max_cnt = 0;
	uint8_t gyroy_wave_max_cnt = 0;
	uint8_t gyroz_wave_max_cnt = 0;

	uint8_t accx_wave_max_cnt = 0;
	uint8_t accy_wave_max_cnt = 0;
	uint8_t accz_wave_max_cnt = 0;
	
	hand_feature_data_init();
	
	for(int i =0;i < p -> queue_length;i++)
	{
		if(p -> queue_gyrox[i] > feature.gyrox_wave_max)
			feature.gyrox_wave_max = hand_int_abs(p -> queue_gyrox[i]);
		
		if(p -> queue_gyroy[i] > feature.gyroy_wave_max)
			feature.gyroy_wave_max = hand_int_abs(p -> queue_gyroy[i]);
		
		if(p -> queue_gyroz[i] > feature.gyroz_wave_max)
			feature.gyroz_wave_max = hand_int_abs(p -> queue_gyroz[i]);
		
		if(p -> queue_ax[i] > feature.accx_wave_max)
			feature.accx_wave_max = hand_int_abs(p -> queue_ax[i]);
		
		if(p -> queue_ay[i] > feature.accy_wave_max)
			feature.accy_wave_max = hand_int_abs(p -> queue_ay[i]);
	
		if(p -> queue_az[i] > feature.accz_wave_max)
			feature.accz_wave_max = hand_int_abs(p -> queue_az[i]);
		
		//
		
		if(hand_int_abs(p -> queue_gyrox[i]) > HAND_GYRO_PEAK_MIN)
			gyrox_wave_max_cnt ++;
		
		if(hand_int_abs(p -> queue_gyroy[i]) > HAND_GYRO_PEAK_MIN)
			gyroy_wave_max_cnt ++;
		
		if(hand_int_abs(p -> queue_gyroz[i]) > HAND_GYRO_PEAK_MIN)
			gyroz_wave_max_cnt ++;
		
		if(hand_int_abs(p -> queue_ax[i]) > HAND_ACCE_PEAK_MIN)
			accx_wave_max_cnt ++;
		
		if(hand_int_abs(p -> queue_ay[i]) > HAND_ACCE_PEAK_MIN)
			accy_wave_max_cnt ++;
		
		if(hand_int_abs(p -> queue_az[i]) > HAND_ACCE_PEAK_MIN)
			accz_wave_max_cnt ++;

		//
		if(gyrox_wave_max_cnt == 2)
		{
			gyrox_wave_max_cnt = 0;
			feature.gyrox_wave_peak_cnt ++;
		}
			
		if(gyroy_wave_max_cnt == 2)
		{
			gyroy_wave_max_cnt = 0;
			feature.gyroy_wave_peak_cnt ++;
		}
		
		if(gyroz_wave_max_cnt == 2)
		{
			gyroz_wave_max_cnt = 0;
			feature.gyroz_wave_peak_cnt ++;
		}
		
		if(accx_wave_max_cnt == 2)
		{
			accx_wave_max_cnt = 0;
			feature.accx_wave_peak_cnt ++;
		}
		
		if(accy_wave_max_cnt == 2)
		{
			accy_wave_max_cnt = 0;
			feature.accy_wave_peak_cnt ++;
		}
		
		if(accz_wave_max_cnt == 2)
		{
			accz_wave_max_cnt = 0;
			feature.accz_wave_peak_cnt ++;
		}
	}
}


void hand_gestures_ana() 
{
	hand_t* p = hand_pointer;

	if((hand_q31_abs(p -> PUT_ANGLE_END_0 - p -> PUT_ANGLE_START_0) >= 20
		&& hand_q31_abs(p -> PUT_ANGLE_START_1 - p -> PUT_ANGLE_END_1) >= 20)) //err
	{
		feature.action = put;
	}
	 
	else if(hand_q31_abs(p -> APPLAUSE_ANGLE_START) >= 50 && hand_q31_abs(p -> APPLAUSE_ANGLE_END) >= 50 
		&& (p -> mean_power > 2000 && p -> mean_power < 7000))
	{
		feature.action = applause;
	}
	
	else if(hand_q31_abs(p -> PALM_ANGLE_END) >= 25
		&& (p -> mean_power > 2300 && p -> mean_power < 10000))
	{
			
		feature.action = palm;
	}
	else 
		feature.action = none;
	
	// printf("%d\n" , (uint8_t)feature.action);
	
}
/*end of hand_exit_or_wave_ana_fun*/

/*hand_state_jump*/
void hand_state_jump()
{
	switch((uint8_t)hand_pointer->cstate)
	{
		case IDLE:
			if(hand_cur_power_calc()) 
				hand_pointer -> cstate = START;

			break;
		
		case START:
			hand_pointer -> cstate = COLLECT;
			break;
		
		case COLLECT:
			if(!hand_cur_power_calc())
			{
				hand_pointer -> cstate = WAIT_EXIT;
				hand_queue_min_init();
			}
			break;
		
		case WAIT_EXIT:
			if(hand_wati_exit_handler() && hand_pointer -> hand_ccr != HAND_QUEUE_MIN_LENGTH)
				hand_pointer -> cstate = WAIT_EXIT;
			else if(hand_pointer -> hand_ccr == HAND_QUEUE_MIN_LENGTH)
				hand_pointer -> cstate = COLLECT;
			else 
				hand_pointer -> cstate = EXIT_AND_ANA;
			break;
		
		case EXIT_AND_ANA://unend
			hand_pointer -> cstate = IDLE;
			break;
	}
}
/*end of hand_state_jump*/


/*hand_state_deal*/
void hand_state_deal()
{
	switch((uint8_t)hand_pointer->cstate)
	{
		case IDLE:
            hand_pointer -> finish_mark = 0;
			hand_get_imu_data();
			break;

		case START:
			led_on();
			hand_start_handler();
			break;

		case COLLECT:
			hand_collect_handler();
			break;
		
		case WAIT_EXIT:
			
			break;
		
		case EXIT_AND_ANA:
			hand_exit_ana_handler();
			led_off();
			hand_pointer -> debug = 1;
            hand_pointer -> finish_mark = 1;
			break;
	}
}
/*end of hand_state_deal*/

