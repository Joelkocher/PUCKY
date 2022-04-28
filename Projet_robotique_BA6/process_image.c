#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>

static float distance_cm = 0;
static float distance_L1 = 0;
static float distance_L8 = 0;
static int angle_de_tournage=0;
static uint16_t line_position = IMAGE_BUFFER_SIZE/2;	//middle


uint16_t get_angle(float distance_L1, float distance_L8){

	if(distance_L1> distance_L8)

		if(distance_L1 > BORNE_MIN_L1_15 && distance_L1 < BORNE_MAX_L8_15 && distance_L8 > BORNE_MIN_L8_15 && distance_L8 < BORNE_MAX_L8_15){
			      angle_de_tournage= DEG_15;
		 }
		if(distance_L1 > BORNE_MIN_L1_30 && distance_L1 < BORNE_MAX_L8_30 && distance_L8 > BORNE_MIN_L8_30 && distance_L8 < BORNE_MAX_L8_30){
					angle_de_tournage= DEG_30;
		 }
		if(distance_L1 > BORNE_MIN_L1_45 && distance_L1 < BORNE_MAX_L8_45 && distance_L8 > BORNE_MIN_L8_45 && distance_L8 < BORNE_MAX_L8_45){
					angle_de_tournage= DEG_45;
		 }
		if(distance_L1 > BORNE_MIN_L1_60 && distance_L1 < BORNE_MAX_L8_60 && distance_L8 > BORNE_MIN_L8_60 && distance_L8 < BORNE_MAX_L8_60){
					angle_de_tournage= DEG_60;
		 }
		if(distance_L1 > BORNE_MIN_L1_75 && distance_L1 < BORNE_MAX_L8_75 && distance_L8 > BORNE_MIN_L8_75 && distance_L8 < BORNE_MAX_L8_75){
					angle_de_tournage= DEG_75;
		}
    if(distance_L1< distance_L8)

    	if(distance_L1 > BORNE_MIN_L1_15 && distance_L1 < BORNE_MAX_L8_15 && distance_L8 > BORNE_MIN_L8_15 && distance_L8 < BORNE_MAX_L8_15){
					angle_de_tournage= -DEG_15;
    	}
    	if(distance_L1 > BORNE_MIN_L1_30 && distance_L1 < BORNE_MAX_L8_30 && distance_L8 > BORNE_MIN_L8_30 && distance_L8 < BORNE_MAX_L8_30){
				     angle_de_tournage= -DEG_30;
    	}
		if(distance_L1 > BORNE_MIN_L1_45 && distance_L1 < BORNE_MAX_L8_45 && distance_L8 > BORNE_MIN_L8_45 && distance_L8 < BORNE_MAX_L8_45){
					angle_de_tournage= -DEG_45;
		 }
		if(distance_L1 > BORNE_MIN_L1_60 && distance_L1 < BORNE_MAX_L8_60 && distance_L8 > BORNE_MIN_L8_60 && distance_L8 < BORNE_MAX_L8_60){
					angle_de_tournage= -DEG_60;
		}
		if(distance_L1 > BORNE_MIN_L1_75 && distance_L1 < BORNE_MAX_L8_75 && distance_L8 > BORNE_MIN_L8_75 && distance_L8 < BORNE_MAX_L8_75){
					angle_de_tournage= -DEG_75;
		}
    if(distance_L1 == distance_L8){
	      	angle_de_tournage= DEG_90;
	    }

}


//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */


static THD_WORKING_AREA(waCaptureImage, 256);
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
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}


static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image[IMAGE_BUFFER_SIZE] = {0};
	uint16_t lineWidth = 0;

	bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts only the red pixels
		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
			//extracts first 5bits of the first byte
			//takes nothing from the second byte
			image[i/2] = (uint8_t)img_buff_ptr[i]&0xF8;
		}

		//search for a line in the image and gets its width in pixels


		//converts the width into a distance between the robot and the camera
		if(lineWidth){
			distance_cm = PXTOCM/lineWidth;
		}

		if(send_to_computer){
			//sends to the computer the image
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}
		//invert the bool
		send_to_computer = !send_to_computer;
    }
}

float get_distance_cm(void){
	return distance_cm;
}

uint16_t get_line_position(void){
	return line_position;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
