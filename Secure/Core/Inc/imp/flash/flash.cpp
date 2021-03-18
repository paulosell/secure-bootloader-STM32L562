/*
 * flash.cpp
 *
 *  Created on: 27 de fev de 2021
 *      Author: sellp
 */

#include "flash.h"
#include "stm32l5xx_hal.h"
#include <string.h>

Flash::Flash() {
}
;

Flash::STATUS_t Flash::flashWrite(DATA_t address, DATA_t word) {

	uint32_t typeProgram;

	typeProgram = (address.word.word_32 < 0x08040000) ? FLASH_TYPEPROGRAM_DOUBLEWORD : FLASH_TYPEPROGRAM_DOUBLEWORD_NS;

	if (this->flashUnlock() == Flash::STATUS_t::SUCCESS) {
		if ( HAL_FLASH_Program(typeProgram,
				address.word.word_32, word.word.word_64) == HAL_OK) {
			return Flash::STATUS_t::SUCCESS;

		} else {

			this->flashLock();

		}
	}

	return Flash::STATUS_t::FAIL;


}

Flash::STATUS_t Flash::flashErase(DATA_t address) {

	uint32_t FirstPage = 0;
	uint32_t BankNumber = 0;
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;

	Flash::STATUS_t result = Flash::STATUS_t::FAIL;

	BankNumber = Flash::GetBank(address.word.word_32);
	FirstPage = Flash::GetPage(address.word.word_32);


	/* Fill EraseInit structure*/
	if(FirstPage >= 128)
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES_NS;
	if(FirstPage < 128)
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = BankNumber;
	EraseInitStruct.Page = FirstPage;
	EraseInitStruct.NbPages = 1;

	if (this->flashUnlock() == Flash::STATUS_t::SUCCESS){
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) {
			result = Flash::STATUS_t::SUCCESS;
		}
	}

	this->flashLock();
	return result;
}


Flash::STATUS_t Flash::flashErase(DATA_t address, size_t num_pages) {

	uint32_t FirstPage = 0;
	uint32_t BankNumber = 0;
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;

	Flash::STATUS_t result = Flash::STATUS_t::FAIL;

	BankNumber = Flash::GetBank(address.word.word_32);
	FirstPage = Flash::GetPage(address.word.word_32);


	/* Fill EraseInit structure*/
	if(FirstPage >= 128)
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES_NS;
		if(FirstPage < 128)
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = BankNumber;
	EraseInitStruct.Page = FirstPage;
	EraseInitStruct.NbPages = (uint32_t) num_pages;

	if (this->flashUnlock() == Flash::STATUS_t::SUCCESS){
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) {
			result = Flash::STATUS_t::SUCCESS;
		}
	}

	this->flashLock();
	return result;
}


Flash::STATUS_t Flash::flashRead(DATA_t address, DATA_t *word_out) {

	switch (word_out->size){
	case SIZE_8:
		word_out->word.word_8 = * (uint8_t *)address.word.word_32;
		break;
	case SIZE_16:
		word_out->word.word_16 = * (uint16_t *)address.word.word_32;
		break;
	case SIZE_32:
		word_out->word.word_32 = * (uint32_t *)address.word.word_32;
		break;
	case SIZE_64:
		word_out->word.word_64 = * (uint64_t *)address.word.word_32;
		break;
	}

	return Flash::STATUS_t::SUCCESS;



}

Flash::STATUS_t Flash::flashLock(void) {

	Flash::STATUS_t result = Flash::STATUS_t::FAIL;

	if (!this->flash_locked) {
		HAL_StatusTypeDef status = HAL_FLASH_Lock();
		if (status == HAL_OK) {
			this->flash_locked = true;
			result = Flash::STATUS_t::SUCCESS;
		} else {
			this->flash_locked = false;
		}
	} else {
		result = Flash::STATUS_t::SUCCESS;
	}

	return result;
}

Flash::STATUS_t Flash::flashUnlock(void) {

	Flash::STATUS_t result = Flash::STATUS_t::FAIL;

	if (this->flash_locked) {
		HAL_StatusTypeDef status = HAL_FLASH_Unlock();
		if (status == HAL_OK) {
			this->flash_locked = false;
			result = Flash::STATUS_t::SUCCESS;
		} else {
			this->flash_locked = true;
		}
	} else {
		result = Flash::STATUS_t::SUCCESS;
	}

	return result;
}

uint32_t Flash::GetPage(uint32_t Addr) {
	uint32_t page = 0;

	if (Addr < (0x08000000 + FLASH_BANK_SIZE)) {
		/* Bank 1 */
		page = (Addr - 0x08000000) / FLASH_PAGE_SIZE;
	} else {
		/* Bank 2 */
		page = (Addr - (0x08000000 )) / FLASH_PAGE_SIZE;
	}

	return page;

}

uint32_t Flash::GetBank(uint32_t Addr) {
	uint32_t bank;

	if (Addr < (0x08000000 + FLASH_BANK_SIZE)) {
		/* Bank 1 */
		bank = FLASH_BANK_1;
	} else {
		/* Bank 2 */
		bank =  FLASH_BANK_2;
	}
	return bank;
}



