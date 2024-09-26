/*
 * Aqi.h
 *
 *  Created on: Jan 18, 2024
 *      Author: Geetansh
 */

#ifndef INC_AQI_H_
#define INC_AQI_H_
/**************************************USER INCLUDES*********************************/
#include "stm32f4xx_hal.h"
#include "main.h"

/**************************************FUNCITON PROTOTYPES***************************/

uint16_t get_Actual_AQI(uint16_t no2_index ,uint16_t co_index, uint16_t pm2_5index, uint16_t pm_10_index);



#endif /* INC_AQI_H_ */
