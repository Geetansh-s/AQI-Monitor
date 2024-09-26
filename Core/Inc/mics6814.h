/*
 * mics6814.h
 *
 *  Created on: Jan 18, 2024
 *      Author: Geetansh
 */

#ifndef INC_MICS6814_H_
#define INC_MICS6814_H_

/*********************************USER INCLUDES*********************************************/
#include "stm32f4xx_hal.h" /*Needed for I2C*/
#include "main.h"

/******************************Private Function Prototypes**********************************/
void airq5_init(void);													//Configure ADC, Recursively calls airq5_setConfiguration

void airq5_writeData(uint8_t reg, uint16_t _data);						//Write data to register

uint16_t airq5_readData(uint8_t reg);									//Read data from register

void airq5_setConfiguration(uint16_t config);							//Configure ADC parameters

uint16_t airq5_readSensorData(uint16_t channel_data);					//Read Gas Voltage Values

uint16_t NO2_volts_to_ppb(uint16_t sensorDat);							//NO2 voltage to ppb conversion

uint16_t NO2_ppbToAQI(uint16_t ppbval);									//NO2 PPB to AQI conversion

uint16_t CO_volts_to_ppm(uint16_t sensorDat);							//CO Volt to PPM conversion

uint16_t CO_ppmToAQI(uint16_t ppmval);									//CO PPM to AQI Conversion

uint16_t getNO2_ppb(void);												//Get Final NO2 PPB

uint16_t getNO2_AQI(void);												//Get Final NO2 AQI

uint16_t getCO_ppm(void);												//Get Final CO PPB

uint16_t getCO_aqi(void);												//Get Final CO AQI


#endif /* INC_MICS6814_H_ */
