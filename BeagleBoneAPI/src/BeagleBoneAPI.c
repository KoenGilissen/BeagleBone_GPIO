/*
 ============================================================================
 Name        : BeagleBoneAPI.c
 Author      : Koen Gilissen
 Version     : 0.1
 Copyright   : GPLv3
 Description : BeagleBone Project
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "gpio.h"

#define _MAXLINE 100
#define _NBR_PINS_USED 8


int main(void)
{
	//----------<Pin configurations>------------
	gpioPin gpioList[_NBR_PINS_USED];
	gpioPin gpio1_6, gpio1_7, gpio1_2, gpio1_13, gpio1_12, gpio0_26, gpio1_15, gpio1_14;

	gpio1_6.HEADER = "P8";
	gpio1_6.NAME = "gpmc_ad6";
	gpio1_6.PHYSPIN = 3;
	gpio1_6.GPIOPIN = "GPIO1_6";
	gpio1_6.GPIONUMBER = (1*32)+6;
	gpioList[0] = gpio1_6;

	gpio1_7.HEADER = "P8";
	gpio1_7.NAME = "gpmc_ad7";
	gpio1_7.PHYSPIN = 4;
	gpio1_7.GPIOPIN = "GPIO1_7";
	gpio1_7.GPIONUMBER = (1*32)+7;
	gpioList[1] = gpio1_7;

	gpio1_2.HEADER = "P8";
	gpio1_2.NAME = "gpmc_ad2";
	gpio1_2.PHYSPIN = 5;
	gpio1_2.GPIOPIN = "GPIO1_2";
	gpio1_2.GPIONUMBER = (1*32)+2;
	gpioList[2] = gpio1_2;

	gpio1_13.HEADER = "P8";
	gpio1_13.NAME = "gpmc_ad13";
	gpio1_13.PHYSPIN = 11;
	gpio1_13.GPIOPIN = "GPIO1_13";
	gpio1_13.GPIONUMBER = (1*32)+13;
	gpioList[3] = gpio1_13;

	gpio1_12.HEADER = "P8";
	gpio1_12.NAME = "gpmc_ad12";
	gpio1_12.PHYSPIN = 12;
	gpio1_12.GPIOPIN = "GPIO1_12";
	gpio1_12.GPIONUMBER = (1*32)+12;
	gpioList[4] = gpio1_12;

	gpio0_26.HEADER = "P8";
	gpio0_26.NAME = "gpmc_ad10";
	gpio0_26.PHYSPIN = 14;
	gpio0_26.GPIOPIN = "GPIO0_26";
	gpio0_26.GPIONUMBER = (0*32)+26;
	gpioList[5] = gpio0_26;

	gpio1_15.HEADER = "P8";
	gpio1_15.NAME = "gpmc_ad15";
	gpio1_15.PHYSPIN = 15;
	gpio1_15.GPIOPIN = "GPIO1_15";
	gpio1_15.GPIONUMBER = (1*32)+15;
	gpioList[6] = gpio1_15;

	gpio1_14.HEADER = "P8";
	gpio1_14.NAME = "gpmc_ad14";
	gpio1_14.PHYSPIN = 16;
	gpio1_14.GPIOPIN = "GPIO1_14";
	gpio1_14.GPIONUMBER = (1*32)+14;
	gpioList[7] = gpio1_14;

	//----------</Pin configurations>------------

	int i = 0;
	char mode[_MAX] = {0x0};
	for(i = 0; i < _NBR_PINS_USED; i++)
	{
		//check and set mode of pin to OMAP_MUX_MODE7
		getPinMode(gpioList[i].NAME, mode);
		if(strcmp(mode, "OMAP_MUX_MODE7") != 0)
		{
			printf("IO %s [%s_%d] mode: %s \t\t\t\t[X]\n", gpioList[i].NAME, gpioList[i].HEADER, gpioList[i].PHYSPIN, mode);
			printf("Setting IO %s [%s_%d] to OMAP_MUX_MODE7\n", gpioList[i].NAME, gpioList[i].HEADER, gpioList[i].PHYSPIN);
			setPinMode(gpioList[i].NAME, "7");
		}
		//RECHECK mode of pins
		getPinMode(gpioList[i].NAME, mode);
		if(strcmp(mode, "OMAP_MUX_MODE7") != 0)
		{
			printf("E: CANNOT SET IO %s [%s_%d] to OMAP_MUX_MODE7\t\t\t\t[X]\n", gpioList[i].NAME, gpioList[i].HEADER, gpioList[i].PHYSPIN);
			return -1;
		}
		else
		{
			printf("IO %s [%s_%d] mode: %s \t\t\t\t[OK]\n", gpioList[i].NAME, gpioList[i].HEADER, gpioList[i].PHYSPIN, mode);
		}
	}

	//Export pins && set direction (out)
	i = 0;
	for(i = 0; i < _NBR_PINS_USED; i++)
	{
		exportGPIOPin(gpioList[i]);
		setDirectionGPIOPin(gpioList[i], "out");
	}

	//Knight Rider ;-)
	int nmbOfLoops = 10;
	int loopCounter = 0;
	for(loopCounter = 0; loopCounter <= nmbOfLoops; loopCounter++)
	{
		i = 0;
		for(i = 0; i < _NBR_PINS_USED; i++)
		{
			if(i == 0)
			{
				setValueGPIOPin(gpioList[0], "1");
				setValueGPIOPin(gpioList[_NBR_PINS_USED-1], "0");
			}
			else
			{
				setValueGPIOPin(gpioList[i], "1");
				setValueGPIOPin(gpioList[i-1], "0");
			}
			usleep(500000);
		}
	}


	//Unexport pins
	i = 0;
	for(i = 0; i < _NBR_PINS_USED; i++)
	{
		unexportGPIOPin(gpioList[i]);
	}
	return 0;
}


