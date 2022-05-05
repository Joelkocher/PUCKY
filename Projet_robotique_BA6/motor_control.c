#include "ch.h"
#include "hal.h"
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>
#include <color_control.h>

#define ANGLE2STEPS_CONST					PI*STEP_CORRECTION_FACTOR*EPUCK_DIAMETER/(4*WHEEL_PERIMETER)
#define	STEP_CORRECTION_FACTOR	90


static BSEMAPHORE_DECL(angle_ready, TRUE);
static BSEMAPHORE_DECL(image_ready_sem, TRUE);
static BSEMAPHORE_DECL(color_ready, TRUE);
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
///---------------------------------------threads-------------------------------------------///


/*static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
        //starts a capture
		//dcmi_capture_start();
		//waits for the capture to be done
		//wait_image_ready();
		//signals an image has been captured
    	capture_couleur();
		chBSemSignal(&image_ready_sem);
    }
}

static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//uint8_t *img_buff_ptr;
	//uint8_t image[IMAGE_BUFFER_SIZE] = {0};
	//uint16_t lineWidth = 0;

	bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);


        if(get_couleur()==ROUGE){
            right_motor_set_speed(0);
            left_motor_set_speed(0);
        }
        else{
        	chBSemSignal(&color_ready);
        }
    }
}*/


static THD_WORKING_AREA(waProximity, 256);
static THD_FUNCTION(Proximity, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;


    while(1){

    	//chBSemWait(&color_ready);

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

    	//capture_couleur();
    	/*if(get_couleur()==ROUGE){
    	    		right_motor_set_speed(0);
    	    		left_motor_set_speed(0);
    	    	}*/
    	//else{


			if(get_calibrated_prox(IR_FRONT_RIGHT)>2000 || get_calibrated_prox(IR_FRONT_LEFT)>2000)
			{
				turn_pucky(turn_angle);
			}
			else{
				left_motor_set_speed(MOTOR_SPEED_L);
				right_motor_set_speed(MOTOR_SPEED_R);
			}
    	//}

    }
}


void motor_control_start(void){

	//chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	//chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
	chThdCreateStatic(waProximity, sizeof(waProximity), NORMALPRIO, Proximity, NULL);
	chThdCreateStatic(waMotorControl, sizeof(waMotorControl), NORMALPRIO, MotorControl, NULL);
}

