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

/*void turn_90_degree(void) {

	//virage à gauche
	if(last_color == ROUGE) {
		right_motor_set_pos(0);
		left_motor_set_pos(NBR_STEP_90_DEGREE);
		right_motor_set_speed(TURN_SPEED);
		left_motor_set_speed(-TURN_SPEED);

		while(right_motor_get_pos()<=NBR_STEP_90_DEGREE && left_motor_get_pos() >=0) {
			__asm__ volatile("nop");
		}
		right_motor_set_speed(0);		//éteint les moteurs après avoir effectué le virage
		left_motor_set_speed(0);
	}

	//virage à droite
	if(last_color == VERT)	{
		right_motor_set_pos(NBR_STEP_90_DEGREE);
		left_motor_set_pos(0);
		right_motor_set_speed(-TURN_SPEED);
		left_motor_set_speed(TURN_SPEED);

		while(right_motor_get_pos()>=0 && left_motor_get_pos()<= NBR_STEP_90_DEGREE) {
			__asm__ volatile("nop");
		}
		right_motor_set_speed(0);
		left_motor_set_speed(0);
	}

	//arrêter les moteurs
	if(last_color == BLEU) {
		motor_stop = true;
	}
}
*/


static THD_WORKING_AREA(waMotorControl, 256);
static THD_FUNCTION(MotorControl, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;


    while(1){

    	int distance_IR1 = 0;
    	int distance_IR8 = 0;
    	double turn_angle = 0;

    	distance_IR1 = get_calibrated_prox(IR_FRONT_RIGHT);
    	distance_IR8 = get_calibrated_prox(IR_FRONT_LEFT);
    	turn_angle = get_angle(turn_angle,distance_IR1,distance_IR8);

        time = chVTGetSystemTime();

        //applies the speed from the PI regulator and the correction for the rotation
        get_angle(turn_angle, distance_IR1, distance_IR8);

        if(get_calibrated_prox(IR_FRONT_RIGHT)>2000 || get_calibrated_prox(IR_FRONT_LEFT)>2000)
       	{
       	turn_pucky(turn_angle);
       	}
      	else{
       		left_motor_set_speed(MOTOR_SPEED_L);
       		right_motor_set_speed(MOTOR_SPEED_R);
      	}
        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}


void motor_control_start(void){
	chThdCreateStatic(waMotorControl, sizeof(waMotorControl), NORMALPRIO, MotorControl, NULL);
}

