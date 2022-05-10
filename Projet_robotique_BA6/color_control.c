/*
 * color_control.c
 *
 *  Created on: May 4, 2022
 *      Author: rheas
 */
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <camera/po8030.h>
#include "main.h"
#include "color_control.h"

//intensités maximales possibles pour chaque canal RGB
#define MAX_VALUE_RED		31
#define MAX_VALUE_GREEN		63
#define MAX_VALUE_BLUE	 	31
#define SLOPE_WIDTH			20
#define RED_GAIN			0x5E		//default 0x5E
#define GREEN_GAIN			0x48		//default 0x40
#define BLUE_GAIN			0x5D		//default 0x5D


//--------------------------------------------------------------------------------------------------------------
//*Déclaration des fonctions et variables globales internes
//--------------------------------------------------------------------------------------------------------------

static uint16_t couleur[4] = {0};	//memorise le nombre de fois que chaque couleur est dominante (rien=0, red = 1, green = 2
									//blue = 3

/* fonction:  detecte la couleur vue par la camera
 * arguments: intensité des 3 canaux RGB.
 * return:    aucun
 */
void detection_couleur(uint16_t red, uint16_t green, uint16_t blue);

/* fonction:  lancer une capture d'image
 * arguments: aucun
 * return:    aucun
 */
//void capture_image(void);

/* fonction:  lit les canaux RGB de l'image pour faire la moyenne de chaque canal
 * arguments: l'adresse du premier element du tableau de taille 3 qui contiendra la moyenne de chaque canal
 * return:    aucun
 */
void lecture_image(uint16_t* moyennes_couleur);

/* fonction:  Trouve la zone colorée et calcule la somme des valeurs de l'image
 * arguments: tableau contenant les valueurs d'intensitées mesurées par la caméra
 * return:    la somme des valeurs de l'intensité dans la zone colorée de image
 */
uint16_t somme_couleur_image(uint8_t* image);

//--------------------------------------------------------------------------------------------------------------
//*implémentations des fonctions
//--------------------------------------------------------------------------------------------------------------

void capture_image(void) {
    //demare une capture
	dcmi_capture_start();
	//attends que la capture soit terminée
	wait_image_ready();
}

//--------------------------------------------------------------------------------------------------------------

void lecture_image(uint16_t* moyennes_couleur){

	uint8_t *img_buff_ptr;

	//gets the pointer to the array filled with the last image in RGB565
	img_buff_ptr = dcmi_get_last_image_ptr();
	uint8_t image[IMAGE_BUFFER_SIZE] = {0};

	//calcul des moyennes normalisées en fonction de leur max de chaque canal cette manière pour éviter dépass. de capa
	//on fait l'opération pour chaque ligne, décalage de i de 2*IMAGE_BUFFER_SIZE
	for(uint8_t j=0; j<3; j++){
		//ROUGE
		for(uint16_t i=0; i<(2*IMAGE_BUFFER_SIZE); i+=2){
			image[i/2]=((uint8_t)img_buff_ptr[i+(j*2*IMAGE_BUFFER_SIZE)] & (0b11111000))>>3;
		}
		*(moyennes_couleur) += somme_couleur_image(image)/MAX_VALUE_RED;

		//VERT
		for(uint16_t i=0; i<(2*IMAGE_BUFFER_SIZE); i+=2){
			image[i/2]=(((uint8_t)img_buff_ptr[i+(j*2*IMAGE_BUFFER_SIZE)] & (0b00000111))<<3) |
					(((uint8_t)img_buff_ptr[i+(j*2*IMAGE_BUFFER_SIZE)+1] & (0b11100000))>>5);
		}
		*(moyennes_couleur + 1) += somme_couleur_image(image)/MAX_VALUE_GREEN;

		//BLEU
		for(uint16_t i=0; i<(2*IMAGE_BUFFER_SIZE); i+=2){
			image[i/2]= ((uint8_t)img_buff_ptr[i+(j*2*IMAGE_BUFFER_SIZE)+1] & (0b00011111));
		}
		*(moyennes_couleur + 2) += somme_couleur_image(image)/MAX_VALUE_BLUE;
	}


}

//------------------------------------------------------------------------------------------------------------

uint16_t somme_couleur_image(uint8_t* image){

	//valeur à renvoyer
	uint16_t somme = 0;

	//variables used in function
	bool stop = 0;
	uint8_t threshold = 0;
	uint16_t mean = 0, i = 0, end = IMAGE_BUFFER_SIZE, start = 0;

	//average to have adaptive threshold
	for(i=0; i<IMAGE_BUFFER_SIZE; i++) {
		mean += image[i];
	}
	threshold = mean/IMAGE_BUFFER_SIZE;
	i = 0;

	//search for beginning of the colored region
	while(i<IMAGE_BUFFER_SIZE-SLOPE_WIDTH && stop==0){
		//rising edge detection
		if (image[i] < threshold && image[i+SLOPE_WIDTH] > threshold){
			start=i;
			stop=1;
		}
		i++;
	}
	stop=0;
	//search for end of the colored region
	while(i<IMAGE_BUFFER_SIZE-SLOPE_WIDTH && stop==0){
		//falling edge detection
		if (image[i] < threshold && image[i+SLOPE_WIDTH] > threshold){
			end=i;
			stop=1;
		}
		i++;
	}

	//sum of all values in the colored zone
	for(i=start; i<end; i++){
		somme += image[i];
	}

	return somme;
}

//-------------------------------------------------------------------------------------------------------------
/* Une couleur est considérée comme dominante si la valeur de son canal correspondant est plus grande que
 * la valeur moyenne de l'intensité capturée. Si plus d'un canal est dominant, la couleur detectée
 * est mise à 0 (aucune couleur n'est détectée)
 *
 */
void detection_couleur(uint16_t red, uint16_t green, uint16_t blue) {

	uint16_t threshold = 0;
	threshold = (red+green+blue)/(3);
	//chprintf((BaseSequentialStream *)&SD3, "\n");
	//chprintf((BaseSequentialStream *)&SD3, "thresh = %d \n ", threshold);


	bool red_dominant = (red >= threshold);
	//chprintf((BaseSequentialStream *)&SD3, "red = %d \n ", red);
	bool green_dominant = (green >= threshold);
	//chprintf((BaseSequentialStream *)&SD3, "green = %d \n ", green);
	bool blue_dominant = (blue >= threshold);
	//chprintf((BaseSequentialStream *)&SD3, "blue = %d \n ", blue);

	if(red_dominant & !green_dominant & !blue_dominant)				couleur[1] += 1;
	else if(!red_dominant & green_dominant & !blue_dominant)		couleur[2] += 1;
	else if(!red_dominant & !green_dominant & blue_dominant)		couleur[3] += 1;
	else 															couleur[0] += 1;
}

//--------------------------------------------------------------------------------------------------------------
//*implémentations des fonctions publiques
//--------------------------------------------------------------------------------------------------------------

void camera_init(void){
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 6, SUBSAMPLING_X1, SUBSAMPLING_X1);
	po8030_set_awb(0);	//disables auto white balance
	po8030_set_ae(1);	//enables auto-exposure
	po8030_set_rgb_gain(RED_GAIN, GREEN_GAIN, BLUE_GAIN);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();
}

//--------------------------------------------------------------------------------------------------------

int get_couleur(void) {
	int dominant = 0;
	if ((couleur [1] > couleur [2]) && (couleur[1] > couleur[3])) 		dominant = 1;
	else if ((couleur [2] > couleur [1]) && (couleur[2] > couleur[3])) 	dominant = 2;
	else if ((couleur [3] > couleur [1]) && (couleur[3] > couleur[2])) 	dominant = 3;
	else																dominant = 0;
	return dominant;
}

//---------------------------------------------------------------------------------------------------------

void reset_couleur(void) {
	for (uint8_t i = 0; i <= 3; i++) couleur[i] = 0;
}

//---------------------------------------------------------------------------------------------------------
void capture_couleur(void) {
	//contiendra la moyenne de chaque canal RGB dans l'ordre Red, Green, Blue
	uint16_t moyennes_image[3] = {0};

	capture_image();
	lecture_image(moyennes_image);
	detection_couleur(moyennes_image[0], moyennes_image[1], moyennes_image[2]);
}





