#include "ch.h"
#include "hal.h"
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>


/* 
	If distance is under min. distance measured by ToF and color is blue, turn Pucky
	Position of motors must be set with the following function:

	1000 steps/s = 1 revolution/s
	1 revolution = PERIMETER_EPUCK = 40.84 cm
	speed 5 cm/s = 122 setps/s

	Wheels must turn according to following formula:
	turn_angle*WHEEL_PERIMETER/360

*/
void turn_pucky(double angle)
{
	left_motor_set_speed(MOTOR_SPEED_L);
	right_moto_set_speed(-MOTOR_SPEED_R);

}
