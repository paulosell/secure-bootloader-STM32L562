/*
 * boot.h
 *
 *  Created on: 2 de mar de 2021
 *      Author: sellp
 */

#ifndef INC_IMP_BOOTLOADER_BOOT_H_
#define INC_IMP_BOOTLOADER_BOOT_H_

#include "interfaces/bootloader/bootloader.h"
#include "imp/flash/flash.h"
#include "imp/comm/usb.h"

class Boot : public Bootloader{
public:

	Boot();
	STATUS_t checkFirmwareIntegrity(void);


	/* Should be used during secure update */
	STATUS_t checkNewFirmwareSignature(void);
	STATUS_t checkNewFirmwareVersion(void);
	STATUS_t receiveNewFirmware(void);
	STATUS_t hasToUpdate(void);

	STATUS_t validateNewFirmware(void);
	STATUS_t finishUpdate(void);
	STATUS_t storeAssets(uint8_t * assets, size_t len);
	STATUS_t forceUpdateFromBootloader();
	STATUS_t isButtonPressed();



};



#endif /* INC_IMP_BOOTLOADER_BOOT_H_ */
