/*
 * Aqi.c
 *
 *  Created on: Jan 24, 2024
 *      Author: Geetansh
 */


/*****************************************************INCLUDES******************************************************/
#include"Aqi.h"
/**************************************************USER VARIABLES****************************************************/
uint16_t master_aqi = 0;
uint16_t largest_aqi[2] = {0};
/*************************************************Function Definitions****************************************************/

/*
 * @brief      Calculates AQI from Gas Data
 * @param[in]  Gas concentrations of CO, NO2, PM2.5, PM10
 * @param[out] AQI
 * @note       none
 */
uint16_t get_Actual_AQI(uint16_t no2_index ,uint16_t co_index, uint16_t pm2_5index, uint16_t pm_10_index)
{
	if(no2_index < co_index)
	{
		largest_aqi[0] = co_index;
	}
	else
	{
		largest_aqi[0] = no2_index;
	}

	if(pm2_5index < pm_10_index)
	{
		largest_aqi[1] = pm_10_index;
	}
	else
	{
		largest_aqi[1] = pm2_5index;

	}

	if(largest_aqi[0] < largest_aqi[1])
	{
		master_aqi = largest_aqi[1] ;
	}
	else
	{
		master_aqi = largest_aqi[0];
	}
	return(master_aqi);
}
