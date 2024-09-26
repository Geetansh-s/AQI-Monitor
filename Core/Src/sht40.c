/*
 * sht40.c
 *
 *  Created on: Jan 20, 2024
 *      Author: Geetansh
 */

/*****************************************************INCLUDES******************************************************/
#include"sht40.h"
#include"stdlib.h"
/****************************************************I2C Handle*****************************************************/

I2C_HandleTypeDef hi2c2;

/**************************************************USER VARIABLES****************************************************/
static uint8_t sht_raddr = 0x89;
static uint8_t sht_waddr = 0x88;

static uint8_t cmd = 0xFD;
int8_t buffer[6] = {0};

uint16_t t_ticks;   //it should remain uint16_t instead of int16 for it causes wrong measurement above 42 degree Centigrade
uint16_t rh_ticks;
int16_t t_degC;
int16_t rh_pRH;

char Temp[3] 	= {"\0"};
char Humid[3] 	= {"\0"};
/*********************************************USER FUNCTION DEFINITIONS*************************************************/

/*
 * @brief      Get Temperature
 * @param[in]  void
 * @param[out] Temperature string
 * @note       none
 */

char* sht_getTemp(void)
{
	//Transmit Command To read Temp and Humidity One Shot
    HAL_I2C_Master_Transmit(&hi2c2, sht_waddr, (uint8_t*)&cmd, 1, HAL_MAX_DELAY);

    HAL_Delay(10);

    //Receive 4 Bytes Temp and Humidity Data with 2 bytes of CRC
    HAL_I2C_Master_Receive(&hi2c2, sht_raddr, (uint8_t*)&buffer, 6, HAL_MAX_DELAY);

    //Conversion formula for integer calculation from datasheet
    t_ticks = ( (buffer[0] * 256) + buffer[1] );

    t_degC = (-45 + (175 * t_ticks/65535) ) ;

    //Convert temperature to string
  	itoa(t_degC, Temp, 10);

  	return(Temp);
}

/*
 * @brief      Get Humidity
 * @param[in]  void
 * @param[out] Temperature string
 * @note       none
 */

char* sht_getHumidity(void)
{
	//Transmit Command To read Temp and Humidity One Shot
    HAL_I2C_Master_Transmit(&hi2c2, sht_waddr, (uint8_t*)&cmd, 1, HAL_MAX_DELAY);

    HAL_Delay(10);

    //Receive 4 Bytes Temp and Humidity Data with 2 bytes of CRC
    HAL_I2C_Master_Receive(&hi2c2, sht_raddr, (uint8_t*)&buffer, 6, HAL_MAX_DELAY);

    //Humidity formula for integer calculation from datasheet
    rh_ticks = ( (buffer[3] * 256) + buffer[4] );

    rh_pRH = (-6 + (125 * rh_ticks/65535) ) ;

    // For RH signal out of range,  cropping of signal   ---> Section 4.6 of datasheet
    if (rh_pRH > 100)
    rh_pRH = 100;
    else if (rh_pRH < 0)
    rh_pRH = 0;

    //Convert temperature to string
 	itoa(rh_pRH, Humid, 10);

 	return(Humid);

}
