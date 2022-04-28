#include <sensors/proximity.h>
#include <proximity_sensors.h>

#define FRONT_RIGHT 0
#define FRONT_LEFT	7

#define NB_OF_ANGLES 7
#define NB_OF_VARIABLES 2


static int distance_cm 		=	0;
static int distance_L1 		=	0;
static int distance_L8 		=	0;
static int 	turn_angle 		=	0;

void get_proximity()
{
	distance_L1 = get_prox(FRONT_RIGHT);
	distance_L8 = get_prox(FRONT_LEFT);
}

void get_angle(int distance_L1, int distance_L8){

	if(distance_L1> distance_L8)

		if(distance_L1 > BORNE_MIN_L1_15 && distance_L1 < BORNE_MAX_L8_15 && distance_L8 > BORNE_MIN_L8_15 && distance_L8 < BORNE_MAX_L8_15){
			     	turn_angle = DEG_15;
		 }
		if(distance_L1 > BORNE_MIN_L1_30 && distance_L1 < BORNE_MAX_L8_30 && distance_L8 > BORNE_MIN_L8_30 && distance_L8 < BORNE_MAX_L8_30){
					turn_angle = DEG_30;
		 }
		if(distance_L1 > BORNE_MIN_L1_45 && distance_L1 < BORNE_MAX_L8_45 && distance_L8 > BORNE_MIN_L8_45 && distance_L8 < BORNE_MAX_L8_45){
					turn_angle = DEG_45;
		 }
		if(distance_L1 > BORNE_MIN_L1_60 && distance_L1 < BORNE_MAX_L8_60 && distance_L8 > BORNE_MIN_L8_60 && distance_L8 < BORNE_MAX_L8_60){
					turn_angle = DEG_60;
		 }
		if(distance_L1 > BORNE_MIN_L1_75 && distance_L1 < BORNE_MAX_L8_75 && distance_L8 > BORNE_MIN_L8_75 && distance_L8 < BORNE_MAX_L8_75){
					turn_angle = DEG_75;
		}
    if(distance_L1< distance_L8)

    	if(distance_L1 > BORNE_MIN_L1_15 && distance_L1 < BORNE_MAX_L8_15 && distance_L8 > BORNE_MIN_L8_15 && distance_L8 < BORNE_MAX_L8_15){
					turn_angle = -DEG_15;
    	}
    	if(distance_L1 > BORNE_MIN_L1_30 && distance_L1 < BORNE_MAX_L8_30 && distance_L8 > BORNE_MIN_L8_30 && distance_L8 < BORNE_MAX_L8_30){
				     turn_angle = -DEG_30;
    	}
		if(distance_L1 > BORNE_MIN_L1_45 && distance_L1 < BORNE_MAX_L8_45 && distance_L8 > BORNE_MIN_L8_45 && distance_L8 < BORNE_MAX_L8_45){
					turn_angle = -DEG_45;
		 }
		if(distance_L1 > BORNE_MIN_L1_60 && distance_L1 < BORNE_MAX_L8_60 && distance_L8 > BORNE_MIN_L8_60 && distance_L8 < BORNE_MAX_L8_60){
					turn_angle = -DEG_60;
		}
		if(distance_L1 > BORNE_MIN_L1_75 && distance_L1 < BORNE_MAX_L8_75 && distance_L8 > BORNE_MIN_L8_75 && distance_L8 < BORNE_MAX_L8_75){
					turn_angle = -DEG_75;
		}
    	if(distance_L1 == distance_L8){
	      			turn_angle = DEG_90;
	    }

}