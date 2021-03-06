/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  /*IO attributes management functions */
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_All, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOC, (GPIO_PIN_All & ~(GPIO_PIN_13)), GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOD, (GPIO_PIN_All & ~(GPIO_PIN_3)), GPIO_PIN_NSEC);;










}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
