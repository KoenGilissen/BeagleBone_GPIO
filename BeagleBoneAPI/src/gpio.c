/*
 ============================================================================
 Name        : GPIO.c
 Author      : Koen Gilissen
 Version     : 0.1
 Copyright   : GPL V3
 Description : BeagleBone GPIO
 ============================================================================
 */
#include "gpio.h"

int exportGPIOPin(gpioPin pin)
{
	FILE *GPIOHandle = NULL;
	char setValue[4], GPIOString[4], GPIOExport[_MAX];
	sprintf(GPIOString, "%d", pin.GPIONUMBER); //e.g. GPIO1_8 becomes 60 -> expecting 60
	sprintf(GPIOExport, "/sys/class/gpio/export");

	if((GPIOHandle = fopen(GPIOExport, "ab")) == NULL) // Append Binairy
	{
		printf("Cannot export GPIO %d \n", pin.GPIONUMBER);
		return 1;
	}
	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, GPIOHandle);
	fclose(GPIOHandle);
	printf("Exported %s [gpio%d] on header %s pin %d\n", pin.NAME, pin.GPIONUMBER, pin.HEADER, pin.PHYSPIN);
	return 0;
}

int setDirectionGPIOPin(gpioPin pin, char *direction)
{
	FILE *GPIOHandle = NULL;
	char setValue[4], GPIOString[4], GPIODirection[_MAX];
	sprintf(GPIOString, "%d", pin.GPIONUMBER); //e.g. GPIO1_8 becomes 60 -> expecting 60
	sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", pin.GPIONUMBER);

	if((GPIOHandle = fopen(GPIODirection, "rb+")) == NULL)
	{
		printf("Cannot Open the GPIO Direction of GPIO%d \n", pin.GPIONUMBER);
		return 1;
	}
	strcpy(setValue, direction);
	fwrite(&setValue, sizeof(char), 3, GPIOHandle);
	printf("Direction set \"%s\" for  %s [gpio%d] on header %s pin %d\n", direction, pin.NAME, pin.GPIONUMBER, pin.HEADER, pin.PHYSPIN);
	fclose(GPIOHandle);
	return 0;
}

int setValueGPIOPin(gpioPin pin, char *value)
{
	FILE *GPIOHandle = NULL;
	char setValue[4], GPIOString[4], GPIOValue[_MAX];
	sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", pin.GPIONUMBER);
	sprintf(GPIOString, "%d", pin.GPIONUMBER); //e.g. GPIO1_8 becomes 60 -> expecting 60

	if((GPIOHandle = fopen(GPIOValue, "rb+")) == NULL)
	{
		printf("Cannot access the GPIO VALUE  %s [gpio%d] on header %s pin %d\n", pin.NAME, pin.GPIONUMBER, pin.HEADER, pin.PHYSPIN);
		return 1;
	}
	strcpy(setValue, value);
	fwrite(&setValue, sizeof(char), 1, GPIOHandle);
	fclose(GPIOHandle);
	return 0;
}

int unexportGPIOPin(gpioPin pin)
{
	FILE *GPIOHandle = NULL;
	char setValue[4], GPIOString[4], GPIOUnexport[_MAX];
	sprintf(GPIOString, "%d", pin.GPIONUMBER); //e.g. GPIO1_8 becomes 60 -> expecting 60
	sprintf(GPIOUnexport, "/sys/class/gpio/unexport");
	if((GPIOHandle = fopen(GPIOUnexport, "ab")) == NULL) // "ab" open binairy file for appending...
	{
		printf("Cannot UNEXPORT the GPIO pin\n");
		return 1;
	}
	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, GPIOHandle);
	fclose(GPIOHandle);
	printf("Unexported %s [gpio%d] on header %s pin %d\n", pin.NAME, pin.GPIONUMBER, pin.HEADER, pin.PHYSPIN);
	return 0;
}

/* Example content of /sys/kernel/debug/omap_mux/gpmc_ad6:
    name: gpmc_ad6.gpio1_6 (0x44e10818/0x818 = 0x0037), b NA, t NA
    mode: OMAP_PIN_OUTPUT | OMAP_MUX_MODE7
    signals: gpmc_ad6 | mmc1_dat6 | NA | NA | NA | NA | NA | gpio1_6 */

int getPinMode(char *pinName, char *Mode1) // expecting something like "gpmc_ad6"
{
    char pinMuxPath[LINE_BUF_SIZE] = {0x0};
    char lineBuffer[LINE_BUF_SIZE] = {0x0};
    char pinConfig[_MAX] = {0x0};
    char pinMode0[_MAX] = {0x0};
    char pinMode1[_MAX] = {0x0};

    FILE *pinMuxMode = NULL;

    sprintf(pinMuxPath, "/sys/kernel/debug/omap_mux/%s", pinName);
    if( (pinMuxMode = fopen(pinMuxPath, "r")) == NULL)
    {
        printf("E: could not reach %s\n",  pinMuxPath);
        return -1;
    }
    else
    {
    	debug("Accessing: [%s]\t\t\t[OK]\n", pinMuxPath);
    }
    while(pinMuxMode!=NULL && fgets(lineBuffer, LINE_BUF_SIZE, pinMuxMode)!=NULL)
    {
      debug("Line read: {%s}\n", lineBuffer);
      if(strlen(pinConfig) == 0)
      {
        int pos0 = strcspn(lineBuffer,"=");
        char* start = &lineBuffer[pos0+2];
        strncpy(pinConfig, start, 6);
        pinConfig[6] = '\0';
        debug("pinConfig: {%s}\n", pinConfig);
      }
      else if(strlen(pinMode0) == 0 && strlen(pinMode1) == 0)
      {
        int pos0 = strcspn(lineBuffer,":");
        int pos1 = strcspn(lineBuffer,"|");
        debug("pos0: %d, pos1: %d\n", pos0, pos1);
        char* start = &lineBuffer[pos0+2];
        strncpy(pinMode0, start, pos1-pos0-3);
        pinMode0[pos1-pos0] = '\0';
        debug("pinMode0: {%s}\n", pinMode0);
        start = &lineBuffer[pos1+2];
        strncpy(pinMode1, start, strlen(lineBuffer)-pos1-3);
        pinMode1[strlen(lineBuffer)-pos1] = '\0';
        debug("pinMode1: {%s}\n", pinMode1);
      }
      else
      {
        break;
      }
    }
    strcpy(Mode1, pinMode1);
    debug("---- RESULTS ----\n");
    debug("pinConfig: %s\n", pinConfig);
    debug("pinMode0: %s\n", pinMode0);
    debug("pinMode1: %s\n-----------------\n\n", pinMode1);
	return 0;
}

int setPinMode(char *pinName, char *mode) // mode "1" ... "7"
{
    char pinMuxPath[LINE_BUF_SIZE] = {0x0};
    char modeSetValue[10] = {0x0};
    FILE *pinMuxMode = NULL;

    sprintf(pinMuxPath, "/sys/kernel/debug/omap_mux/%s", pinName);
    if( (pinMuxMode = fopen(pinMuxPath, "w")) == NULL)
    {
        printf("E: could not reach %s\n",  pinMuxPath);
        return -1;
    }
    else
    {
    	debug("Accessing: [%s]\t\t\t[OK]\n", pinMuxPath);
    }
    strcpy(modeSetValue, mode);
    debug(printf("Setting to mode %s\n", modeSetValue));
	fwrite(&modeSetValue, sizeof(char), strlen(modeSetValue), pinMuxMode);
	fclose(pinMuxMode);
	return 0;
}

