#include "jumper.h"
#include "stm32l5xx_hal.h"
#include "main.h"
void Jumper::goToNonSecure(){
	HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_All, GPIO_PIN_NSEC);
	funcptr_NS NonSecure_ResetHandler;

	  SCB_NS->VTOR = APP_START_ADDRESS;

	  /* Set non-secure main stack (MSP_NS) */
	  __TZ_set_MSP_NS((*(uint32_t *)APP_START_ADDRESS));

	  /* Get non-secure reset handler */
	  NonSecure_ResetHandler = (funcptr_NS)(*((uint32_t *)((APP_START_ADDRESS) + 4U)));

	  /* Start non-secure state software application */
	  NonSecure_ResetHandler();

}
