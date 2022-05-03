#include "ch.h"
#include "hal.h"
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>

#define ANGLE2STEPS_CONST					PI*STEP_CORRECTION_FACTOR*EPUCK_DIAMETER/(4*WHEEL_PERIMETER)
#define	STEP_CORRECTION_FACTOR	90


static BSEMAPHORE_DECL(angle_ready, TRUE);
/* 
	If distance is under min. distance measured by proximity sensor and color is blue, 
	-- > turn Pucky

	
	Constant that determines how many steps corresponds to which angle of incidence:

	ANGLE2STEPS_CONST = PI*STEP_CORRECTION_FACTOR*EPUCK_DIAMETER/(4*WHEEL_PERIMETER)

	The motors must turn by step_goal each to turn the right amount

*/

void turn_pucky(double angle)
{
	unsigned int step_goal;
	step_goal = abs(ANGLE2STEPS_CONST*angle/45);
	if(angle > 0)
	{
		right_motor_set_pos(0);
		left_motor_set_pos(step_goal);
		right_motor_set_speed(MOTOR_SPEED_R);
		left_motor_set_speed(-MOTOR_SPEED_L);

		while(right_motor_get_pos()<=step_goal && left_motor_get_pos() >=0) {
			__asm__ volatile("nop");
		}
		right_motor_set_speed(0);
		left_motor_set_speed(0);
	}
	if(angle < 0)
	{
		right_motor_set_pos(step_goal);
		left_motor_set_pos(0);
		right_motor_set_speed(-MOTOR_SPEED_R);
		left_motor_set_speed(MOTOR_SPEED_L);
		while(left_motor_get_pos()<=step_goal && right_motor_get_pos() >=0) {
			__asm__ volatile("nop");
		}
		right_motor_set_speed(0);
		left_motor_set_speed(0);
	}

}

static THD_WORKING_AREA(waProximity, 256);
static THD_FUNCTION(Proximity, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;


    while(1){

    	distance_IR1 = get_calibrated_prox(IR_FRONT_RIGHT);
    	distance_IR8 = get_calibrated_prox(IR_FRONT_LEFT);
    	turn_angle = get_angle(turn_angle,distance_IR1,distance_IR8);

    	chBSemSignal(&angle_ready);

    }
}

static THD_WORKING_AREA(waMotorControl, 256);
static THD_FUNCTION(MotorControl, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	chBSemWait(&angle_ready);
		//time = chVTGetSystemTime();



		if(get_calibrated_prox(IR_FRONT_RIGHT)>2000 || get_calibrated_prox(IR_FRONT_LEFT)>2000)
		{
		   turn_pucky(turn_angle);
		 }
		else{
		   left_motor_set_speed(MOTOR_SPEED_L);
		   right_motor_set_speed(MOTOR_SPEED_R);
		 }

    }
}


void motor_control_start(void){
	chThdCreateStatic(waProximity, sizeof(waProximity), NORMALPRIO, Proximity, NULL);
	chThdCreateStatic(waMotorControl, sizeof(waMotorControl), NORMALPRIO, MotorControl, NULL);
}

