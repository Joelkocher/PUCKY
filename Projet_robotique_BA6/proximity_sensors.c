#include <sensors/proximity.h>
#include <proximity_sensors.h>

#define FRONT_RIGHT 0
#define FRONT_LEFT	7

#define NB_OF_ANGLES 7
#define NB_OF_VARIABLES 2

static const int angle_table[NB_OF_ANGLES][NB_OF_VARIABLES] ={
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
}


void get_angle()
{
	unsigned int a;
	unsigned int b;

	//a = get_calibrated_prox(FRONT_RIGHT);
	//b = get_calibrated_prox(FRONT_LEFT);

	a = get_prox(FRONT_RIGHT);
	b = get_prox(FRONT_LEFT);



}