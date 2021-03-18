/*
 * bootloader.h
 *
 *  Created on: 17 de fev de 2021
 *      Author: sellp
 */

#ifndef INTERFACES_BOOTLOADER_BOOTLOADER_H_
#define INTERFACES_BOOTLOADER_BOOTLOADER_H_

#include <iostream>
#include "../flashmanager/flashman.h"
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

class Bootloader{
public:

	enum STATE_t{
		BOOTLOADER = 0x01,
		APP = 0x02,
		VALIDATION = 0x03,
		UPDATE_PREPARATION = 0x04,
		UPDATE_RECEPTION = 0x05,
		UPDATE_VERIFY = 0x06,
		UPDATE_INSTALLATION = 0x07
	};

	enum COMMAND_t{
		ACK = 0x00,
		RESET = 0x01,
		STATE = 0x02,
		APP_UPDATE = 0x03,
		BOOTLOADER_START = 0x04,
		BOOTLOADER_WRITE = 0x05,
		BOOTLOADER_DONE = 0x06,
		ERROR = 0x08
	};


	enum STATUS_t{
		FAIL = 0,
		SUCCESS = 1
	};


	Bootloader();
	/* Should be used during secure boot */
	virtual STATUS_t checkFirmwareIntegrity(void);
	virtual STATUS_t checkFirmwareSignature(void);
	virtual STATUS_t checkFirmwareVersion(void);

	/* Should be used during secure update */
	virtual STATUS_t checkNewFirmwareIntegrity(void);
	virtual STATUS_t checkNewFirmwareSignature(void);
	virtual STATUS_t checkNewFirmwareVersion(void);

	virtual STATUS_t hasToUpdate(void);


	STATE_t getState(void);
	STATUS_t setState(STATE_t state);

	STATUS_t getIsReadyToBoot(void);
	STATUS_t setIsReadyToBoot(STATUS_t status);



private:

	STATE_t state;
	STATUS_t is_ready_to_boot;
};




#endif /* INTERFACES_BOOTLOADER_BOOTLOADER_H_ */
