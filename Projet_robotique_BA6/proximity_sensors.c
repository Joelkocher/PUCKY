#include <sensors/proximity.h>
#include <proximity_sensors.h>

#define IR_FRONT_RIGHT		0
#define IR_FRONT_LEFT		7

#define SQUARE_COEFF		0.038
#define LIN_COEFF			-3.827
#define CONST_COEFF			93.788


/*
Angle is approximated with a quadratic function.
It depends on ratio = distance_IR1/distance_IR8:

angle(ratio)=0.038*ratio^2 - 3.827*ratio + 93.788

*/


void get_proximity(int distance1, int distance2)
{
	distance1 = get_calibrated_prox(IR_FRONT_RIGHT);
	distance2 = get_calibrated_prox(IR_FRONT_LEFT);
}

/*
	get_angle is the function that gives the measured angle

	If the angle is on the left of the e-puck, it's value will be negative
	Otherwise it will be positive
*/
double get_angle(int distance1, int distance2){

	//ratio between 
	double ratio;
	double inv_ratio;
	double turn_angle = 0;

	ratio = distance1/distance2;
	inv_ratio = distance2/distance1;

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
