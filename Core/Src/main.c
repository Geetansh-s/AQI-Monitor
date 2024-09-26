/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include "mics6814.h"
#include "ens160.h"
#include "sht40.h"
#include "ZPH_03.h"
#include "fonts.h"
#include "ssd1306.h"
#include "Aqi.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

/* USER CODE BEGIN PV */
uint16_t NO2_ppb;
uint16_t CO_ppm;
uint16_t CO2_ppm;
uint16_t TVOC_ppb;
uint16_t NH3_ppm;

uint16_t pm_2_5;
uint16_t pm_10;
uint16_t PM_2_5_subIndex;
uint16_t PM_10_subIndex;
uint16_t finalAQI = 0;

uint16_t NO2_AQI;
uint16_t CO_AQI;

char CO2_Quality[9] 	= {"\0"};
char Temperature[3] 	= {"\0"};
char Humidity[3] 		= {"\0"};

char atrespond[20] = {};

char local_buf[100] = {0};

char key[] = {"YOCGY86U5AT7MBLW"};
/*
char pm2_5buf[10] = {"\0"};
char pm10_buf[10] = {"\0"};
*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*Module Initializations*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart5, (uint8_t*)"AT+RST\r\n", sizeof("AT+RST\r\n"), 1000);
  HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
  HAL_Delay(3000);


  HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CWJAP=\"Geetansh Galaxy S23\",\"8076251649\"\r\n", sizeof("AT+CWJAP=\"Geetansh Galaxy S23\",\"8076251649\"\r\n"), 1000);
  HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);


  airq5_init();
  ens160_Init();
  ZPH03_Init();
  SSD1306_Init();  // initialise

  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("Please Wait", &Font_11x18, 1);
  SSD1306_UpdateScreen(); //display

  HAL_Delay(30000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		 //--------------------------------GET THE DATA--------------------------------

		 NO2_ppb =  getNO2_ppb();
		 NO2_AQI =  getNO2_AQI();
		 CO_ppm  =  getCO_ppm();
		 CO_AQI  =  getCO_aqi();


		 CO2_ppm = ens_getCO2_PPM();

		 strcpy(CO2_Quality, (ens_getCO2_Quality(CO2_ppm)));

		 strcpy(Temperature, (sht_getTemp()));
		 strcpy(Humidity, (sht_getHumidity()));

		 pm_2_5 = ZPH03_PM_2_5();
		 pm_10 	= ZPH03_PM_10();

		 PM_2_5_subIndex = ZPH03_pm_2_5_getSubindex();
		 PM_10_subIndex  = ZPH03_pm_2_5_getSubindex();

		 finalAQI = get_Actual_AQI(NO2_AQI, CO_AQI, PM_2_5_subIndex, PM_10_subIndex);

		 //--------------------------------DISPLAY THE DATA--------------------------------

		 //Convert Integer To string

		  	char pm2_5_buf[5];
		  	itoa(pm_2_5, pm2_5_buf, 10);

			char pm10_buf[5];
			itoa(pm_10, pm10_buf, 10);

			char co_buf[5];
			itoa(CO_ppm, co_buf, 10);

			char no2_buf[5];
			itoa(NO2_ppb, no2_buf, 10);


		  	char masterAQI[5];
		  	itoa(finalAQI, masterAQI, 10);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (30,0);
		    SSD1306_Puts ("AQI ", &Font_16x26, 1);

		    SSD1306_GotoXY (30,30);
		    SSD1306_Puts (masterAQI, &Font_16x26, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (30,0);
		    SSD1306_Puts ("PM 2.5", &Font_11x18, 1);

		    SSD1306_GotoXY (30,20);
		    SSD1306_Puts (pm2_5_buf, &Font_11x18, 1);

		    SSD1306_GotoXY (30,40);
		    SSD1306_Puts ("ug/m3", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (30,0);
		    SSD1306_Puts ("PM 10", &Font_11x18, 1);

		    SSD1306_GotoXY (30,20);
		    SSD1306_Puts (pm10_buf, &Font_11x18, 1);

		    SSD1306_GotoXY (30,40);
		    SSD1306_Puts ("ug/m3", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (30,0);
		    SSD1306_Puts ("CO gas", &Font_11x18, 1);

		    SSD1306_GotoXY (30,20);
		    SSD1306_Puts (co_buf, &Font_11x18, 1);

		    SSD1306_GotoXY (30,40);
		    SSD1306_Puts ("PPM", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (30,0);
		    SSD1306_Puts ("NO2", &Font_11x18, 1);

		    SSD1306_GotoXY (30,20);
		    SSD1306_Puts (no2_buf, &Font_11x18, 1);

		    SSD1306_GotoXY (30,40);
		    SSD1306_Puts ("PPB", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);


		  	SSD1306_Clear();
		    SSD1306_GotoXY (0,0);
		    SSD1306_Puts ("CO2 Quality", &Font_11x18, 1);

		    SSD1306_GotoXY (0,30);
		    SSD1306_Puts (CO2_Quality, &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);

		  	SSD1306_Clear();
		    SSD1306_GotoXY (0,0);
		    SSD1306_Puts ("Temp :", &Font_11x18, 1);
		    SSD1306_GotoXY (50,0);
		    SSD1306_Puts (Temperature, &Font_11x18, 1);
		    SSD1306_GotoXY (80,0);
		    SSD1306_Puts (" C", &Font_11x18, 1);
		    SSD1306_GotoXY (0,30);
		    SSD1306_Puts ("RH", &Font_11x18, 1);
		    SSD1306_GotoXY (50,30);
		    SSD1306_Puts (Humidity, &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    HAL_Delay(5000);


			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", sizeof("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSEND=51\r\n", sizeof("AT+CIPSEND=51\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			sprintf (local_buf, "GET /update?api_key=%s&field%d=%u\r\n", key, 1, finalAQI);
			int len = strlen (local_buf);

			HAL_UART_Transmit(&huart5, (uint8_t*)local_buf, len, 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(8000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
		    HAL_Delay(8000);
			/*___________________________________________________________________________________________________________________________________________________________________________*/
			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", sizeof("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSEND=51\r\n", sizeof("AT+CIPSEND=51\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			sprintf (local_buf, "GET /update?api_key=%s&field%d=%u\r\n", key, 2, pm_2_5);
			 len = strlen (local_buf);

			HAL_UART_Transmit(&huart5, (uint8_t*)local_buf, len, 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(8000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
		    HAL_Delay(8000);
			/*___________________________________________________________________________________________________________________________________________________________________________*/
			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", sizeof("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSEND=51\r\n", sizeof("AT+CIPSEND=51\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			sprintf (local_buf, "GET /update?api_key=%s&field%d=%u\r\n", key, 3, pm_10);
			 len = strlen (local_buf);

			HAL_UART_Transmit(&huart5, (uint8_t*)local_buf, len, 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(8000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
		    HAL_Delay(8000);
			/*___________________________________________________________________________________________________________________________________________________________________________*/
			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", sizeof("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSEND=51\r\n", sizeof("AT+CIPSEND=51\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			sprintf (local_buf, "GET /update?api_key=%s&field%d=%u\r\n", key, 4, CO_ppm);
			 len = strlen (local_buf);

			HAL_UART_Transmit(&huart5, (uint8_t*)local_buf, len, 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(8000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
		    HAL_Delay(8000);
			/*___________________________________________________________________________________________________________________________________________________________________________*/
			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", sizeof("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPSEND=51\r\n", sizeof("AT+CIPSEND=51\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(3000);

			sprintf (local_buf, "GET /update?api_key=%s&field%d=%u\r\n", key, 5, NO2_ppb);
			 len = strlen (local_buf);

			HAL_UART_Transmit(&huart5, (uint8_t*)local_buf, len, 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);
			HAL_Delay(8000);

			HAL_UART_Transmit(&huart5, (uint8_t*)"AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n"), 1000);
			HAL_UART_Receive(&huart5, (uint8_t*)atrespond, sizeof(atrespond), 1000);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
