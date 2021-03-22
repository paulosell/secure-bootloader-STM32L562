#include "main.h"
#include "gtzc.h"
#include "gpio.h"
#include "clk_core.h"
#include "imp/bootloader/boot.h"
#include "imp/switcher/jumper.h"

int main(void) {
	HAL_Init();

	SystemClock_Config();

	MX_GTZC_S_Init();

	MX_GPIO_Init();

	Boot b;
	Jumper j;


	if (b.isButtonPressed() == Boot::STATUS_t::SUCCESS){
		b.forceUpdateFromBootloader();
	}

	if (b.hasToUpdate()) {
		while (b.updateFirmware() == Boot::STATUS_t::FAIL)
			;
	}

	b.setState(Boot::STATE_t::VALIDATION);

	while (b.checkFirmwareIntegrity() != Boot::STATUS_t::SUCCESS) {
		b.setState(Boot::STATE_t::BOOTLOADER);
		b.updateFirmware();
	}

	j.goToNonSecure();

	/*never reach here*/
	while (1) {

	}

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
