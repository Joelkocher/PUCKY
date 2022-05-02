#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
	#endif

	#include "camera/dcmi_camera.h"
	#include "msgbus/messagebus.h"
	#include "parameter/parameter.h"
	#include "math.h"


	//constants for the differents parts of the project
	#define IMAGE_BUFFER_SIZE		640
	#define WIDTH_SLOPE				5
	#define MIN_LINE_WIDTH			40
	#define ROTATION_THRESHOLD		10
	#define ROTATION_COEFF			2
	#define PXTOCM					1570.0f //experimental value
	#define GOAL_DISTANCE 			10.0f
	#define MAX_DISTANCE 			25.0f
	#define ERROR_THRESHOLD			0.1f	//[cm] because of the noise of the camera
	#define KP						800.0f
	#define KI 						3.5f	//must not be zero
	#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)
	#define WHEEL_PERIMETER         13 // [cm]
	#define PI                  	3.1415926536f
	#define WHEEL_DISTANCE      	5.35f    //cm


	#define DISTANCE_LIMITE_VIRAGE  1
	#define PERIMETER_EPUCK     	(PI * WHEEL_DISTANCE)
	#define TURN_FACTOR				360

	#define SPEED_R					5
	#define SPEED_L					5

	#define SQUARE_COEFF		0.038
	#define LIN_COEFF			-3.827
	#define CONST_COEFF			93.788

	static int distance_cm 		=	0;
	static int distance_IR1		=	0;
	static int distance_IR8		=	0;
	static int turn_angle 		=	0;

	/** Robot wide IPC bus. */
	extern messagebus_t bus;

	extern parameter_namespace_t parameter_root;

	void SendUint8ToComputer(uint8_t* data, uint16_t size);

	#ifdef __cplusplus
}
#endif

#endif

