/*
 * ens160.h
 *
 *  Created on: Jan 19, 2024
 *      Author: Geetansh
 */

#ifndef INC_ENS160_H_
#define INC_ENS160_H_

/**************************************USER INCLUDES*********************************/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

/************************************* Private Function Prototypes******************/

void ens160_Init(void);					//Initialize ENS 160 Sensor

uint16_t ens_getCO2_PPM(void);			//Get CO2 concentration in PPM

uint16_t ens_getTVOC_PPB(void);			//Get TVOC concentration in PPB

char* ens_getCO2_Quality(uint16_t);		//Get CO2 air quality


#endif /* INC_ENS160_H_ */
