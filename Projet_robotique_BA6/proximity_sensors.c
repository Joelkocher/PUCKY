#include <sensors/proximity.h>
#include <proximity_sensors.h>
#include <sensors/VL53L0X/VL53L0X.h>

#include <main.h>

#define SQUARE_COEFF		0.038
#define LIN_COEFF		    -3.827
#define CONST_COEFF			93.788

/*
	get_angle is the function that gives the measured angle

	If the angle is on the left of the e-puck, it's value will be negative
	Otherwise it will be positive

	turn_angle is determined by the ratio between the distances measured by IR1 and IR8:

	turn_angle = 0.038*ratio*ratio-3.827*ratio+93.788
*/
double get_angle(double turn_angle, int distance_IR1, int distance_IR8){


	//ratio between measured distances
	double ratio;
	double inv_ratio;

	ratio = distance_IR1/ distance_IR8;
	inv_ratio = distance_IR8/ distance_IR1;

	if(ratio>=1)
	{
		turn_angle = SQUARE_COEFF*ratio*ratio+LIN_COEFF*ratio+CONST_COEFF;

		//If the angle is too small, the angle is set to 45 degrees to avoid bad behavior while turning
		if (turn_angle < MINIMUM_ANGLE){
			turn_angle = MINIMUM_ANGLE;
		}
	}
	else
	{
		turn_angle = -(SQUARE_COEFF*inv_ratio*inv_ratio+LIN_COEFF*inv_ratio+CONST_COEFF);
		if (turn_angle > -MINIMUM_ANGLE){
					turn_angle = -MINIMUM_ANGLE;
		}
	}
	return turn_angle;
}


