/*
 * flash.h
 *
 *  Created on: 27 de fev de 2021
 *      Author: sellp
 */

#ifndef INC_IMP_FLASH_FLASH_H_
#define INC_IMP_FLASH_FLASH_H_

#include <iostream>
#include "interfaces/flashmanager/flashman.h"

class Flash : public FlashManager {

public:
	Flash();

	STATUS_t flashWrite(DATA_t address, DATA_t word);
	STATUS_t flashErase(DATA_t address);
	STATUS_t flashRead(DATA_t address, DATA_t *word_out);
	STATUS_t flashLock(void);
	STATUS_t flashUnlock(void);
	STATUS_t flashErase(DATA_t address, size_t num_pages);



private:
	bool flash_locked;
	static uint32_t GetPage(uint32_t Addr);
	static uint32_t GetBank(uint32_t Addr);


};

#endif /* INC_IMP_FLASH_FLASH_H_ */
