/*
 * gpio.h
 *
 *  Created on: Mar 19, 2013
 *      Author: wizard
 */

#ifndef GPIO_H_
#define GPIO_H_

//__INCLUDES___________________________//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//__DEFINES___________________________//
#define LINE_BUF_SIZE 128
#define _MAX 64

//#define DEBUG

#ifdef DEBUG
  #define debug(args...) printf(args)
#else
  #define debug(...) ((void)0)
#endif



//__TYPEDEFS_STRUCTS___________________________//
typedef struct
{
	char *HEADER;			//e.g.: P8 / P9
	int PHYSPIN;			//e.g.: 3 / 44
	char *NAME;				//e.g.: gpmc_ad6 / mcasp0_ahclkx
	char *GPIOPIN; 			//e.g.: gpio1[6] / gpio3[17]
	int GPIONUMBER;     	//e.g.: 38
}gpioPin;


//__FUNCTION PROTOTYPES___________________________//

int exportGPIOPin(gpioPin);
int setDirectionGPIOPin(gpioPin, char *);
int setValueGPIOPin(gpioPin, char *);
int unexportGPIOPin(gpioPin);
int getPinMode(char *, char *);
int setPinMode(char *, char *);

#endif /* GPIO_H_ */
