//#ifndef PROXIMITY_SENSORS_H
//#define PROXIMITY_SENSORS_H

#include <main.h>

#define IR_FRONT_RIGHT		0
#define IR_FRONT_LEFT		7


//void get_proximity();

double get_angle(double turn_angle, int distance_IR1, int distance_IR8);
