#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <msgbus/messagebus.h>
#include <i2c_bus.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <sensors/proximity.h>
#include <math.h>

#include <pi_regulator.h>
#include <process_image.h>

void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{

    halInit();
    chSysInit();
    //mpu_init();

    //message bus init.
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    //starts the serial communication
    serial_start();
    //start the USB communication
    usb_start();
    //starts the camera
    //dcmi_start();
	//po8030_start();

    //initialize proximity sensors
	proximity_start();
	//messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");

	calibrate_ir();

	while(1){

	unsigned int a;
	unsigned int b;

	//a = get_calibrated_prox(0);
	//b = get_calibrated_prox(7);

	a = get_prox(0);
	b = get_prox(7);

	double sinangle;
	double angle;

	double inv_a = 1000;
	//if(a > 0)
	inv_a = 4096-(double)a;

	double inv_b = 1000;
	//if(b > 0)
	inv_b = 4096-(double)b;

	sinangle = (0.2956*inv_a/(inv_a*inv_a+inv_b*inv_b+1.91*inv_a*inv_b));
	//sinangle = a + b;
	//angle = inv_a + inv_b;
	angle = asin(sinangle);


	chprintf((BaseSequentialStream *)&SD3, "distance a = %d \n", a);
	chprintf((BaseSequentialStream *)&SD3, "distance b = %d \n", b);
	chprintf((BaseSequentialStream *)&SD3, "angle = %f \n", angle);
	chprintf((BaseSequentialStream *)&SD3, "sinus de l'angle = %f \n", sinangle);
	chprintf((BaseSequentialStream *)&SD3, "1/a = %f \n", inv_a);
	chprintf((BaseSequentialStream *)&SD3, "1/b = %f \n", inv_b);

	chThdSleepMilliseconds(100);
	}

	/*
	//inits the motors
	motors_init();

	//stars the threads for the pi regulator and the processing of the image
	pi_regulator_start();
	process_image_start();
	*/

    /* Infinite loop. */
    /*while (1) {
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }*/
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
