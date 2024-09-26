/*
 * ZPH_03.h
 *
 *  Created on: Jan 20, 2024
 *      Author: Geetansh
 */

#ifndef INC_ZPH_03_H_
#define INC_ZPH_03_H_

/**************************************USER INCLUDES*********************************/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

/************************************* Private Function Prototypes******************/

void ZPH03_Init(void);							//Initialize sensor for Question and answer mode

uint16_t ZPH03_PM_2_5(void);					//Get PM 2.5 Concentration in ug/m3

uint16_t ZPH03_PM_10(void);						//Get PM 10 Concentration in ug/m3

uint16_t ZPH03_pm_2_5_getSubindex(void);	//Get PM 2.5 Sub index

uint16_t ZPH03_pm_10_getSubindex(void);		//Get PM 10 Sub index


#endif /* INC_ZPH_03_H_ */
