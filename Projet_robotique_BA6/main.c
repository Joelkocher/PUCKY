#include <stdio.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"

#include <main.h>
#include <motors.h>
#include <msgbus/messagebus.h>
#include <camera/po8030.h>
#include <sensors/proximity.h>


#include <process_image.h>
#include <proximity_sensors.h>
#include <motor_control.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //message bus init.
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    //starts the camera
    dcmi_start();
	po8030_start();

    //initialize proximity sensors
	proximity_start();
	calibrate_ir();

	//initialize the motors
	motors_init();

	motor_control_start();
	process_image_start();

	while(1){

	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
