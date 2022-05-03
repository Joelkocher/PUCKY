#include <sensors/proximity.h>
#include <proximity_sensors.h>
#include <sensors/VL53L0X/VL53L0X.h>

#include <main.h>


<<<<<<< HEAD
#define SQUARE_COEFF		0.038
#define LIN_COEFF			-3.827
#define CONST_COEFF			93.788

=======
#define NB_OF_ANGLES 7
#define NB_OF_VARIABLES 2
#define OFFSET 34 //[mm]
>>>>>>> f0d0e86 (.)

/*
Angle is approximated with a quadratic function.
It depends on ratio = distance_IR1/distance_IR8:

angle(ratio)=0.038*ratio^2 - 3.827*ratio + 93.788

static BSEMAPHORE_DECL(distance_reached, TRUE);
static BSEMAPHORE_DECL(angle_captured, TRUE);



void init_distance(void)
{
	VL53L0X_start();
}
uint16_t get_target_distance(void)
{
	return cos(get_angle())*(VL53L0X_get_dist_mm()-OFFSET);
}

void get_proximity()
{
	distance_IR1 = get_calibrated_prox(IR_FRONT_RIGHT);
	distance_IR8 = get_calibrated_prox(IR_FRONT_LEFT);
}
*/

/*
	get_angle is the function that gives the measured angle

	If the angle is on the left of the e-puck, it's value will be negative
	Otherwise it will be positive
*/
double get_angle(double turn_angle, int distance_IR1, int distance_IR8){

	/*
	distance_IR1 = get_calibrated_prox(IR_FRONT_RIGHT);
	distance_IR8 = get_calibrated_prox(IR_FRONT_LEFT);
	*/

	//ratio between
	double ratio;
	double inv_ratio;

<<<<<<< HEAD
	ratio = distance_IR1/ distance_IR8;
	inv_ratio = distance_IR8/ distance_IR1;

	if(ratio>=1)
	{
		turn_angle = SQUARE_COEFF*ratio*ratio+LIN_COEFF*ratio+CONST_COEFF;
	}
	else
	{
		turn_angle = -(SQUARE_COEFF*inv_ratio*inv_ratio+LIN_COEFF*inv_ratio+CONST_COEFF);
	}
	return turn_angle;
=======
}

static THD_WORKING_AREA(waTOF,0);
static THD_FUNCTION(TOF,arg) {

	chRegSetThreadName(__FUNCTION__);
	    (void)arg;



	while(1){

    	 get_target_distance;
    	 chBSemSignal(&distance_reached);
     }
}

static THD_WORKING_AREA(waProximity,0);
static THD_FUNCTION(Proximity,arg) {

	  chRegSetThreadName(__FUNCTION__);
	    (void)arg;


     while(1){

    	 chBSemWait(&distance_reached);
    	 get_proximity();
    	 get_angle(distance_L1,distance_L8);

    	 //thread 3 capture color et turn pucky dedans (moteur) turn_pucky();
    	 chBSemSignal(&angle_captured);
     }
}

static THD_WORKING_AREA(waCouleur,0);
static THD_FUNCTION(Couleur,arg) {

	chRegSetThreadName(__FUNCTION__);
	    (void)arg;



	while(1){

		chBSemWait(&angle_captured);

     }
}


void proximity_and_tof_start(void){
	chThdCreateStatic(waProximity, sizeof(waProximity), NORMALPRIO, Proximity, NULL);
	chThdCreateStatic(waTOF, sizeof(waTOF), NORMALPRIO, TOF, NULL);
	chThdCreateStatic(waCouleur, sizeof(waCouleur), NORMALPRIO, TOF, NULL);
>>>>>>> f0d0e86 (.)
}
