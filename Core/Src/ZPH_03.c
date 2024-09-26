/*
 * ZPH_03.c
 *
 *  Created on: Jan 20, 2024
 *      Author: Geetansh
 */

/*****************************************************INCLUDES******************************************************/
#include"ZPH_03.h"
#include"stdlib.h"
/****************************************************UART Handle*****************************************************/

UART_HandleTypeDef huart4;
/**************************************************USER VARIABLES****************************************************/

uint8_t qna_mode_cmd[] = {0XFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
uint8_t getdata[] = {0XFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t rxbuf[9] = {0};
uint16_t pm_2_5 = 0;
uint16_t pm_10 = 0;
uint16_t pm_2_5_subindex = 0;
uint16_t pm_10_subindex = 0;
/*********************************************USER FUNCTION DEFINITIONS*************************************************/

/*
 * @brief      Initialize the sensor in QNA Mode
 * @param[in]  void
 * @param[out] void
 * @note       none
 */

void ZPH03_Init(void)
{
	HAL_UART_Transmit(&huart4, (uint8_t*)qna_mode_cmd, sizeof(qna_mode_cmd), HAL_MAX_DELAY);
}

/*
 * @brief      Get PM 2.5 Concentration
 * @param[in]  void
 * @param[out] PM 2.5 Concentration String
 * @note       none
 */

uint16_t ZPH03_PM_2_5(void)
{
	//Transmit command to ask for Data
    HAL_UART_Transmit(&huart4, (uint8_t*)getdata, sizeof(getdata), HAL_MAX_DELAY);

    //Receive Data Bytes
	HAL_UART_Receive(&huart4,(uint8_t*)rxbuf, 9, HAL_MAX_DELAY);

	//Convert data to PM 2.5 Information According to Datasheet Formula
	pm_2_5   =  ((rxbuf[2]*256)+ rxbuf[3]) ;

 	return(pm_2_5);
}

/*
 * @brief      Get PM 10 Concentration
 * @param[in]  void
 * @param[out] PM 10 Concentration String
 * @note       none
 */

uint16_t ZPH03_PM_10(void)
{
	//Transmit command to ask for Data
	HAL_UART_Transmit(&huart4, (uint8_t*)getdata, sizeof(getdata), HAL_MAX_DELAY);

    //Receive Data Bytes
	HAL_UART_Receive(&huart4,(uint8_t*)rxbuf, 9, HAL_MAX_DELAY);

	//Convert data to PM 2.5 Information According to Datasheet Formula
	pm_10    =  ((rxbuf[4]*256)+ rxbuf[5]) ;

 	return(pm_10);

}

/*
 * @brief      Get PM 2.5 sub index
 * @param[in]  void
 * @param[out] PM 2.5 sub index
 * @note       none
 */

uint16_t ZPH03_pm_2_5_getSubindex(void)
{
	uint16_t ugm3 = pm_2_5;

	if(ugm3 <= 30)
	{
		pm_2_5_subindex  = 50;
	}
	else if(ugm3 > 30 && ugm3 <= 60 )
	{
		pm_2_5_subindex = 100;
	}
	else if(ugm3 > 60 && ugm3 <= 90)
	{
		pm_2_5_subindex = 150;
	}
	else if(ugm3 > 90 && ugm3 <= 120)
	{
		pm_2_5_subindex = 250;
	}
	else if(ugm3 > 120 && ugm3 <= 250)
	{
		pm_2_5_subindex = 350;
	}
	else if(ugm3 > 250)
	{
		pm_2_5_subindex = 400;
	}

	return(pm_2_5_subindex);
}

/*
 * @brief      Get PM 10 sub index
 * @param[in]  void
 * @param[out] PM 10 sub index
 * @note       none
 */

uint16_t ZPH03_pm_10getSubindex(void)
{

	uint16_t _ugm3 = pm_10;

	if(_ugm3 <= 50)
	{
		pm_10_subindex  = 50;
	}
	else if(_ugm3 > 50 && _ugm3 <= 100 )
	{
		pm_10_subindex = 100;
	}
	else if(_ugm3 > 100 && _ugm3 <= 250)
	{
		pm_10_subindex = 150;
	}
	else if(_ugm3 > 250 && _ugm3 <= 350)
	{
		pm_10_subindex = 250;
	}
	else if(_ugm3 > 350 && _ugm3 <= 430)
	{
		pm_10_subindex = 350;
	}
	else if(_ugm3 > 430)
	{
		pm_10_subindex = 400;
	}

	return(pm_10_subindex);
}
