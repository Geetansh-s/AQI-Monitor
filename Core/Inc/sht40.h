/*
 * sht40.h
 *
 *  Created on: Jan 20, 2024
 *      Author: Geetansh
 */

#ifndef INC_SHT40_H_
#define INC_SHT40_H_

/**************************************USER INCLUDES*********************************/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

/************************************* Private Function Prototypes******************/

//Get Temperature in character buffer
char* sht_getTemp(void);

//Get humidity in character buffer
char* sht_getHumidity(void);


#endif /* INC_SHT40_H_ */
