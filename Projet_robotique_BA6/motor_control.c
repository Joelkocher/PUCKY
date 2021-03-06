#include "ch.h"
#include "hal.h"
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>

#define ANGLE2STEPS_CONST		PI*STEP_CORRECTION_FACTOR*EPUCK_DIAMETER/(4*WHEEL_PERIMETER)
#define	STEP_CORRECTION_FACTOR	85			//Value determined by experimentation
#define TURN_THRESHOLD			1900		//As of this proximity value, robot will turn or stop


static BSEMAPHORE_DECL(angle_ready, TRUE);
/*
 * turn_pucky is the function that makes the robot turn with a given angle
 * Argument: angle of rotation
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

    //If the robot is close to a wall and if the color of the wall is red --> motors stop
    //If the robot is close to a wall and if the color of the wall isn't red --> robot turns
    //If the robot is not close to a wall --> robot goes straight
    while(1){
    	chBSemWait(&angle_ready);

			if(get_calibrated_prox(IR_FRONT_RIGHT)>TURN_THRESHOLD || get_calibrated_prox(IR_FRONT_LEFT)>TURN_THRESHOLD)
			{
				if (pi_get_state()==GAME_ON){
				turn_pucky(turn_angle);
				}
				else{
           		right_motor_set_speed(0);
           		left_motor_set_speed(0);
           		chThdSleepSeconds(4);
       			}
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

