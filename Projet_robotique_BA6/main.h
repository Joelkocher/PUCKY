#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
	#endif

	#include "camera/dcmi_camera.h"
	#include "msgbus/messagebus.h"
	#include "parameter/parameter.h"



	//constants for the differents parts of the project
	#define IMAGE_BUFFER_SIZE		640

	#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)
	#define WHEEL_PERIMETER         13 // [cm]
	#define WHEEL_DISTANCE      	5.35f    //cm
	#define EPUCK_DIAMETER			54		//distance between the wheels

	#define MOTOR_SPEED_L			450	// steps/s = 5 cm/s
	#define MOTOR_SPEED_R			450	// steps/s = 5 cm/s
	#define PI						3.1415926536f

	static int distance_IR1;
	static int distance_IR8;
	static double turn_angle;
	static uint32_t mean;


	/** Robot wide IPC bus. */
	extern messagebus_t bus;

	extern parameter_namespace_t parameter_root;

	void SendUint8ToComputer(uint8_t* data, uint16_t size);

	#ifdef __cplusplus
}
#endif

#endif

