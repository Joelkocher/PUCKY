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


/* 
	If distance is under min. distance measured by ToF and color is blue, turn Pucky
	Position of motors must be set with the following function:

	turn_angle*PERIMETER_EPUCK/360
*/
void turn_pucky(int turn_angle)
{
	motor_set_position(turn_angle*PERIMETER_EPUCK/TURN_FACTOR,-turn_angle*PERIMETER_EPUCK/TURN_FACTOR,SPEER_R,-SPEED_L);
}
