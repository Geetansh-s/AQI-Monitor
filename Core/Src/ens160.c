/*
 * ens160.c
 *
 *  Created on: Jan 19, 2024
 *      Author: Geetansh
 */


/*****************************************************INCLUDES******************************************************/
#include"ens160.h"

/****************************************************I2C Handle*****************************************************/

I2C_HandleTypeDef hi2c2;

/**************************************************USER VARIABLES****************************************************/
// Use 8-bit address
static const uint8_t ENS160_ADDR = 0x53 << 1;

// Register addresses
static const uint8_t REG_OPMODE = 0x10;
static const uint8_t REG_DATA_TVOC = 0x22;
static const uint8_t REG_DATA_CO2 = 0x24;

// Register values
static const uint8_t OPMODE_STANDARD = 0x02;

//Data Buffers
uint8_t TVOCbuf[2];
uint8_t CO2buf[2];
uint8_t buf[24];
uint16_t CO2_value;
uint16_t tvocval;
uint8_t _qCO2;

char co2Quality[9] = {"\0"};
/*********************************************USER FUNCTION DEFINITIONS*************************************************/

/*
 * @brief      Initializes ENS 160 For operation
 * @param[in]  void
 * @param[out] void
 * @note       none
 */

void ens160_Init(void)
{
	  // Change the sensor mode to standard mode
	  buf[0] = REG_OPMODE;
	  buf[1] = OPMODE_STANDARD;
	  HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
}

/*
 * @brief      Get CO2 Concentration in PPM
 * @param[in]  void
 * @param[out] CO2 Concentration in PPM
 * @note       none
 */


uint16_t ens_getCO2_PPM(void)
{
	  //Transmit Command To get CO2 Values From CO2 Register
	  CO2buf[0] = REG_DATA_CO2;

	  HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, CO2buf, 1, HAL_MAX_DELAY);

	  HAL_I2C_Master_Receive(&hi2c2, ENS160_ADDR, CO2buf, 2, HAL_MAX_DELAY);

	  // combines two 8-bit data into one 16-bit data, LSB Received First With Data Right Aligned, LSB | with MSB << 8 positions, Little Endian

	  CO2_value = ((CO2buf[0] | CO2buf[1]<<8));

	  return(CO2_value);
}

/*
 * @brief      Get TVOC Concentration in PPB
 * @param[in]  void
 * @param[out] TVOC Concentration in PPB
 * @note       none
 */

uint16_t ens_getTVOC_PPB(void)
{

	  //Transmit Command To get TVOC Values From TVOC Register
	  TVOCbuf[0] = REG_DATA_TVOC;

	  HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, TVOCbuf, 1, HAL_MAX_DELAY);

	  HAL_I2C_Master_Receive(&hi2c2, ENS160_ADDR, TVOCbuf, 2, HAL_MAX_DELAY);

	  // combines two 8-bit data into one 16-bit data, LSB Received First With Data Right Aligned, LSB | with MSB << 8 positions, Little Endian

	  tvocval = ((TVOCbuf[0] | TVOCbuf[1]<<8));

	  return(tvocval);
}

/*
 * @brief      Get CO2 Quality
 * @param[in]  void
 * @param[out] char pointer with Quality
 * @note       none
 */

char* ens_getCO2_Quality(uint16_t co2ppm)
{
	if(co2ppm <= 400)
	{
		 strcpy(co2Quality, "GOOD");
	}

	else if(co2ppm > 400 && co2ppm <= 1000)
	{
		 strcpy(co2Quality, "MODERATE");
	}

	else if(co2ppm > 1000)
	{
		 strcpy(co2Quality, "POOR");
	}

	return(co2Quality);

}
