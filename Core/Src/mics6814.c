/*
 * mics6814.c
 *
 *  Created on: Jan 18, 2024
 *      Author: Geetansh
 */

/***************************INCLUDES****************************************/
#include"mics6814.h"
/*************************USER VARIABLES***************************************/

/* I2C Read And Write address */
const uint8_t raddr 								= 0x91;
const uint8_t waddr 								= 0x90;

/* ADC Channels */
const uint16_t _AIRQ5_DATA_CHANNEL_NO2 				= 0x4000;
const uint16_t _AIRQ5_DATA_CHANNEL_NH3 				= 0x5000;
const uint16_t _AIRQ5_DATA_CHANNEL_CO  				= 0x6000;

/* Register  */
const uint8_t _AIRQ5_REG_POINTER_CONVERT     		= 0x00;
const uint8_t _AIRQ5_REG_POINTER_CONFIG     	    = 0x01;

/* Configuration OS  */
const uint16_t _AIRQ5_CONFIG_OS_SINGLE   	 		= 0x8000;

/* Configuration PGA  */
const uint16_t _AIRQ5_CONFIG_PGA_2_048V  		 	= 0x0400;

/* Configuration MODE  */
const uint16_t _AIRQ5_CONFIG_SINGLE_MODE     		= 0x0100;

/* Configuration DR  */
const uint16_t _AIRQ5_CONFIG_DATA_RATE_1600SPS   	= 0x0080;

/* Configuration CMODE  */
const uint16_t _AIRQ5_CONFIG_COMP_MODE_TRADITIONAL  = 0x0000;

/* Configuration CPOL  */
const uint16_t _AIRQ5_CONFIG_COMP_POL_ACTIVE_LOW    = 0x0000;

/* Configuration CLAT  */
const uint16_t _AIRQ5_CONFIG_COMP_LAT_NOT_LATCH   	= 0x0000;

/* Configuration CQUE  */
const uint16_t _AIRQ5_CONFIG_COMP_QUE_0CONV  		= 0x0003;

/*I2C Handle*/
I2C_HandleTypeDef hi2c2;

/*Configuration Register Reset to remove previous config*/
static uint16_t _dataConfig = 0x8583;

/*Variables to store Voltages*/

uint16_t NO2_sensorData;
uint16_t NH3_sensorData;
uint16_t CO_sensorData;

/*Variables to store Gas PPM/PPB Data*/
uint16_t NO2_ppb;
uint16_t CO_ppm;
uint16_t NH3_ppm;

/*Variables to store Gas AQI Data*/

uint16_t NO2_AQI;
uint16_t CO_AQI;
uint16_t NH3_AQI;

/*********************************************USER FUNCTION DEFINITIONS*************************************************/

/*
 * @brief      Calls airq5_setConfiguration
 * @param[in]  void
 * @param[out] void
 * @note       none
 */

void airq5_init(void)
{
	  airq5_setConfiguration( _AIRQ5_CONFIG_OS_SINGLE |							//Start Single Shot Conversion
	                              _AIRQ5_CONFIG_PGA_2_048V |					//Set ADC Threshold Between 0 and 2.048 Volts and clip the signal beyond bounds
	                              _AIRQ5_CONFIG_SINGLE_MODE |					//Single Shot mode select
	                              _AIRQ5_CONFIG_DATA_RATE_1600SPS |				//Sample rate 1600s per second
	                              _AIRQ5_CONFIG_COMP_MODE_TRADITIONAL |			//Default Comparator Mode
	                              _AIRQ5_CONFIG_COMP_POL_ACTIVE_LOW |			//Polarity of ready pin for interrupt
	                              _AIRQ5_CONFIG_COMP_LAT_NOT_LATCH |			//Do not latch Ready Pin
	                              _AIRQ5_CONFIG_COMP_QUE_0CONV );				//Pull Ready pin high after one successful Reading

}

/*
 * @brief      Write data to ADC register
 * @param[in]  Register address
 * @param[in]  Data to be written
 * @param[out] void
 * @note       none
 */

void airq5_writeData(uint8_t reg, uint16_t _data)
{
    uint8_t writeReg[ 3 ];

    writeReg[ 0 ] = reg;					//Register To write
    writeReg[ 1 ] = _data >> 8;				// Store MSB, MSB of data right shifted to eliminate LSB from 16 bit variable to accomodate into an 8 bit variable
    writeReg[ 2 ] = _data & 0x00FF;			//Store LSB, LSB & 0xFF = LSB,   MSB & 0x00 = 0x00

    HAL_I2C_Master_Transmit(&hi2c2, waddr, (uint8_t*)&writeReg, 3, HAL_MAX_DELAY);


}

/*
 * @brief      Read voltage from ADC
 * @param[in]  Register address
 * @param[out] ADC Voltage
 * @note       none
 */

uint16_t airq5_readData(uint8_t reg)
{
    uint8_t writeReg[ 1 ];				//One byte Register Address
    uint8_t readReg[ 2 ];				//Two Bytes Return Value
    uint16_t dataValue;

    //Address of register to read data from
    writeReg[ 0 ] = reg;

    HAL_I2C_Master_Transmit(&hi2c2, waddr, (uint8_t*)&writeReg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c2, raddr, (uint8_t*)&readReg, 2, HAL_MAX_DELAY);

    //Store Two 8 bit values into one 16 bit variable : Shift MSB to leftmost and bitwise or with LSB, Data right Aligned, Big Endian
    dataValue = readReg[ 0 ];
    dataValue = dataValue << 8;
    dataValue = dataValue | readReg[ 1 ];

    return dataValue;
}

/*
 * @brief      Store ADC configuration bits in a variable
 * @param[in]  ADC Configuration register parameters
 * @param[out] void
 * @note       none
 */

void airq5_setConfiguration(uint16_t config)
{
	//Stores Data Configuration Values
    _dataConfig = config;
}

/*
 * @brief      Reads ADC voltage on single channel
 * @param[in]  ADC Input Channel
 * @param[out] Voltage
 * @note       none
 */

uint16_t airq5_readSensorData(uint16_t channel_data)
{
    uint16_t setConfig;
    uint16_t getData;

    //Get ADC Config Data
    setConfig = _dataConfig;

    //Combine ADC config with GAS INPUT CHANNEL DATA
    setConfig = setConfig | channel_data;

    //Write config values to config register
    airq5_writeData(_AIRQ5_REG_POINTER_CONFIG, setConfig );

    //Get ADC Voltage Values
    getData = airq5_readData( _AIRQ5_REG_POINTER_CONVERT );

    //ADC returns 12 bit data, Accomodate data in 16 bit variable
    getData = getData >> 4;

    //Return voltage
    return getData;
}

/*
 * @brief      Convert Voltage To PPB for NO2
 * @param[in]  ADC Voltage
 * @param[out] Gas Concentration in PPB
 * @note       none
 */

uint16_t NO2_volts_to_ppb(uint16_t sensorDat)
{
	uint16_t ppbval;
	if(sensorDat >= 1400 && sensorDat <= 2047)
	{
	    ppbval = 100;
	}
	else if(sensorDat >= 1300 && sensorDat <1400)
	{
		ppbval = 200;
	}
	else if(sensorDat >= 1200 && sensorDat < 1300)
	{
		ppbval = 250;
	}
	else if(sensorDat >=1000 && sensorDat < 1200)
	{
		ppbval = 350;
	}
	else if(sensorDat >=800 && sensorDat < 1000)
	{
		ppbval = 500;
	}
	else if(sensorDat >=400 && sensorDat < 800)
	{
		ppbval = 1000;
	}
	else if(sensorDat < 400)
	{
		ppbval = 2000;
	}

	return(ppbval);
}

/*
 * @brief      Convert PPB to AQI for NO2
 * @param[in]  Gas Concentration in PPB
 * @param[out] Gas AQI
 * @note       none
 */

uint16_t NO2_ppbToAQI(uint16_t ppbval)
{
	uint16_t NO2_AQI;

	if(ppbval == 100)
	{
		NO2_AQI  = 95;
	}
	else if(ppbval == 200)
	{
		NO2_AQI = 115;
	}
	else if(ppbval == 250)
	{
		NO2_AQI = 125;
	}
	else if(ppbval == 350)
	{
		NO2_AQI = 150;
	}
	else if(ppbval == 500)
	{
		NO2_AQI = 180;
	}
	else if(ppbval == 1000)
	{
		NO2_AQI = 290;
	}
	else if(ppbval == 2000)
	{
		NO2_AQI = 500;
	}

	return(NO2_AQI);
}

/*
 * @brief      Convert Voltage To PPM for CO
 * @param[in]  ADC Voltage
 * @param[out] Gas Concentration in PPM
 * @note       none
 */

uint16_t CO_volts_to_ppm(uint16_t sensorDat)
{
	uint16_t ppmval;

	if(sensorDat >= 0 && sensorDat <=600)
	{
		ppmval = 1;
	}
	else if(sensorDat >= 700 && sensorDat < 1400)
	{
		ppmval = 4;
	}
	else if(sensorDat >= 1400 && sensorDat <2100)
	{
		ppmval = 9;
	}
	else if(sensorDat >= 2100)
	{
		ppmval = 30;
	}

	if(ppmval == 58755)
	{
		ppmval = 1;
	}
	return(ppmval);
}

/*
 * @brief      Convert PPM to AQI for CO
 * @param[in]  Gas Concentration in PPM
 * @param[out] Gas AQI
 * @note       none
 */

uint16_t CO_ppmToAQI(uint16_t ppmval)
{
		uint16_t CO_AQI;

	if(ppmval == 1)
	{
		CO_AQI  = 95;
	}
	else if(ppmval == 4)
	{
		CO_AQI = 115;
	}
	else if(ppmval == 9)
	{
		CO_AQI = 125;
	}
	else if(ppmval == 30)
	{
		CO_AQI = 300;
	}

	return(CO_AQI);
}

/*
 * @brief      Get NO2 Concentration In PPB
 * @param[in]  void
 * @param[out] Gas Concentration in PPB
 * @note       none
 */

uint16_t getNO2_ppb(void)
{
  NO2_sensorData = (airq5_readSensorData(_AIRQ5_DATA_CHANNEL_NO2));
  NO2_ppb = NO2_volts_to_ppb(NO2_sensorData);
  return(NO2_ppb);
}

/*
 * @brief      Get NO2 AQI
 * @param[in]  void
 * @param[out] NO2 AQI
 * @note       none
 */

uint16_t getNO2_AQI(void)
{
  NO2_AQI = NO2_ppbToAQI(NO2_ppb);
  NH3_sensorData = airq5_readSensorData(_AIRQ5_DATA_CHANNEL_NH3);
  return (NO2_AQI);
}

/*
 * @brief      Get CO Concentration In PPM
 * @param[in]  void
 * @param[out] Gas Concentration in PPM
 * @note       none
 */

uint16_t getCO_ppm(void)
{
CO_sensorData = (airq5_readSensorData(_AIRQ5_DATA_CHANNEL_CO));
CO_ppm = CO_volts_to_ppm(CO_sensorData);
return(CO_ppm);
}

/*
 * @brief      Get CO AQI
 * @param[in]  void
 * @param[out] CO AQI
 * @note       none
 */

uint16_t getCO_aqi(void)
{
	CO_AQI = CO_ppmToAQI(CO_ppm);
	return(CO_AQI);
}

/**********************************END**************************************/
