#include <sensors/proximity.h>
#include <proximity_sensors.h>
#include <sensors/VL53L0X/VL53L0X.h>

#include <main.h>



#define SQUARE_COEFF		0.038
#define LIN_COEFF		    -3.827
#define CONST_COEFF			93.788


#define NB_OF_ANGLES 7
#define NB_OF_VARIABLES 2
#define OFFSET 34 //[mm]




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

}


