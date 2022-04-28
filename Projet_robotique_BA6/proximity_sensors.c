#include <sensors/proximity.h>
#include <proximity_sensors.h>

#define FRONT_RIGHT 0
#define FRONT_LEFT	7

#define NB_OF_ANGLES 7
#define NB_OF_VARIABLES 2


static float distance_cm 		=	0;
static float distance_L1 		=	0;
static float distance_L8 		=	0;
static int angle_de_tournage	=	0;

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