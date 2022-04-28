#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>


// if distance is under min. distance measured by ToF and color is blue, turn pucky

void turn_pucky(int turn_angle, int distance_L1, int distance_L8)
{
	if(turn_angle == DEG_15)
	{
		motor_set_position(DEG_15, DEG_15, SPEED_R, -SPEED_L);
	}
	if(turn_angle == DEG_30)
	{
		motor_set_position(DEG_30, DEG_30, SPEED_R, -SPEED_L);
	}
	if(turn_angle == DEG_45)
	{
		motor_set_position(DEG_45, DEG_45, SPEED_R, -SPEED_L);
	}
	if(turn_angle == DEG_60)
	{
		motor_set_position(DEG_60, DEG_60, SPEED_R, -SPEED_L);
	}
	if(turn_angle == DEG_75)
	{
		motor_set_position(DEG_75, DEG_75, SPEED_R, -SPEED_L);
	}
	if(turn_angle == DEG_15)
	{
		motor_set_position(DEG_90, DEG_90, SPEED_R, -SPEED_L);
	}

	if(turn_angle == -DEG_15)
	{
		motor_set_position(DEG_15, DEG_15, -SPEED_R, SPEED_L);
	}
	if(turn_angle == -DEG_30)
	{
		motor_set_position(DEG_30, DEG_30, -SPEED_R, SPEED_L);
	}
	if(turn_angle == -DEG_45)
	{
		motor_set_position(DEG_45, DEG_45, -SPEED_R, SPEED_L);
	}
	if(turn_angle == -DEG_60)
	{
		motor_set_position(DEG_60, DEG_60, -SPEED_R, SPEED_L);
	}
	if(turn_angle == -DEG_75)
	{
		motor_set_position(DEG_75, DEG_75, -SPEED_R, SPEED_L);
	}

}
