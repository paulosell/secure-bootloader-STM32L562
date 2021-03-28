/*
 * flashman.cpp
 *
 *  Created on: 16 de fev de 2021
 *      Author: sellp
 */


#include "flashman.h"
#include <string.h>
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/


/* USER MIGHT INITILIALIZE CLASS DUE TO PLATFORM NEEDS */
FlashManager::FlashManager(){
	this->flash_locked = false;
}

/* MUST BE IMPLEMENTED BY USER*/
FlashManager::STATUS_t FlashManager::flashWrite(DATA_t address, DATA_t word){
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
FlashManager::STATUS_t FlashManager::flashErase(DATA_t address){
	return STATUS_t::SUCCESS;
}


/* MUST BE IMPLEMENTED BY USER*/
FlashManager::STATUS_t FlashManager::flashLock(void){
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
FlashManager::STATUS_t FlashManager::flashUnlock(void){
	return STATUS_t::SUCCESS;
}

bool FlashManager::isFlashLocked() {
	return this->flash_locked;
}

void FlashManager::setFlashLocked(bool flashLocked) {
	this->flash_locked = flashLocked;
}

FlashManager::STATUS_t FlashManager::flashReadAll(DATA_t address, uint8_t * out, size_t out_len){

	memcpy((void*) out, (const void*) address.word.word_32, out_len);

}
