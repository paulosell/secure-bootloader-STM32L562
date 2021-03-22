/*
 * boot.cpp
 *
 *  Created on: 2 de mar de 2021
 *      Author: sellp
 */

#include <imp/storage/storage.h>
#include "boot.h"
#include "imp/cryptography/crypto-rsa.h"
#include "imp/storage/storage.h"
#include "stm32l562e_discovery.h"

Boot::Boot() {
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
}

Boot::STATUS_t Boot::forceUpdateFromBootloader(){
	uint32_t address = ((uint32_t)0x0803D800);
	HAL_FLASH_Unlock();

	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;


	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = 1;
	EraseInitStruct.Page = 123;
	EraseInitStruct.NbPages = 1;


	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	uint64_t update = 0x0100000000000000;

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address,update);

	HAL_FLASH_Lock();

};

Boot::STATUS_t Boot::isButtonPressed(){
	if(BSP_PB_GetState(BUTTON_USER) == SET){
		return Boot::STATUS_t::SUCCESS;
	}

	return Boot::STATUS_t::FAIL;
}

Boot::STATUS_t Boot::hasToUpdate(void) {

	Storage s;
	uint8_t update[8];
	Boot::STATUS_t result = Boot::STATUS_t::FAIL;
	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);

	if (update[7]) {

		result = Boot::STATUS_t::SUCCESS;
		this->setState(Boot::STATE_t::UPDATE_PREPARATION);
	}

	return result;
}

Boot::STATUS_t Boot::storeAssets(uint8_t *assets, size_t len) {
	uint8_t hash[32];
	uint8_t sig[256];
	uint8_t size[8];
	uint8_t version[8];

	if (len != 304) {
		return Boot::STATUS_t::FAIL;
	}

	int i;
	for (i = 0; i < 32; i++) {
		hash[i] = assets[i];
	}

	for (i = 32; i < 288; i++) {
		sig[i - 32] = assets[i];
	}

	for (i = 288; i < 296; i++) {
		size[i - 288] = assets[i];
	}

	for (i = 296; i < 304; i++) {
		version[i - 296] = assets[i];
	}

	Storage s;
	DATA_t address;

	address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;
	for (int i = 0; i < 32; i += 8) {
		DATA_t word;
		word.word.word_64 = ((uint64_t) hash[i + 7] << 56)
						| ((uint64_t) hash[i + 6] << 48)
						| ((uint64_t) hash[i + 5] << 40)
						| ((uint64_t) hash[i + 4] << 32)
						| ((uint64_t) hash[i + 3] << 24)
						| ((uint64_t) hash[i + 2] << 16) | ((uint64_t) hash[i + 1] << 8)
						| ((uint64_t) hash[i] << 0);

		if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;

	}

	address.word.word_32 = NEW_FIRMWARE_SIGNATURE_ADDRESS;

	for (int i = 0; i < 256; i += 8) {
		DATA_t word;
		word.word.word_64 = ((uint64_t) sig[i + 7] << 56)
						| ((uint64_t) sig[i + 6] << 48) | ((uint64_t) sig[i + 5] << 40)
						| ((uint64_t) sig[i + 4] << 32) | ((uint64_t) sig[i + 3] << 24)
						| ((uint64_t) sig[i + 2] << 16) | ((uint64_t) sig[i + 1] << 8)
						| ((uint64_t) sig[i] << 0);

		if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;
	}

	address.word.word_32 = NEW_FIRMWARE_SIZE_ADDRESS;

	DATA_t word;
	word.word.word_64 = ((uint64_t) size[7] << 56) | ((uint64_t) size[6] << 48)
					| ((uint64_t) size[5] << 40) | ((uint64_t) size[4] << 32);

	if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	address.word.word_32 = NEW_FIRMWARE_VERSION_ADDRESS;

	word.word.word_64 = ((uint64_t) version[7] << 56);

	if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	return Boot::STATUS_t::SUCCESS;
}

Boot::STATUS_t Boot::updateFirmware(void) {
	USBCDC usb;
	BSP_LED_Init(LED9);
	BSP_LED_On(LED9);
	this->setState(Boot::STATE_t::UPDATE_PREPARATION);
	Flash f;
	DATA_t address;
	address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;
	f.flashErase(address);
	address.word.word_32 = NEW_APP_START_ADDRESS;
	f.flashErase(address, 63);
	this->setState(Boot::STATE_t::UPDATE_RECEPTION);

	int reception_finished = 0;
	while (!reception_finished) {

		if (usb.hasData()) {
			uint8_t recv[2048] = { 0 };
			size_t len = 0;
			uint8_t reply = Boot::COMMAND_t::ACK;
			usb.getData(recv, &len);

			switch (recv[0]) {
			case Boot::COMMAND_t::STATE:{
				reply = (uint8_t) this->getState();
				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_START: {

				address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;
				f.flashErase(address);
				address.word.word_32 = NEW_APP_START_ADDRESS;
				f.flashErase(address, 63);

				uint8_t assets[len];
				memcpy((void*) assets, (const void*) recv + 5, len);
				if (this->storeAssets(assets, len) != Boot::STATUS_t::SUCCESS)
					reply = Boot::COMMAND_t::ERROR;
				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_WRITE: {
				uint8_t fw[len];
				memcpy((void*) fw, (const void*) recv + 5, len);

				for (int i = 0; i < len; i += 8) {
					DATA_t word;

					word.word.word_64 = ((uint64_t) fw[i + 7] << 56)
									| ((uint64_t) fw[i + 6] << 48)
									| ((uint64_t) fw[i + 5] << 40)
									| ((uint64_t) fw[i + 4] << 32)
									| ((uint64_t) fw[i + 3] << 24)
									| ((uint64_t) fw[i + 2] << 16)
									| ((uint64_t) fw[i + 1] << 8)
									| ((uint64_t) fw[i] << 0);

					if (f.flashWrite(address, word)
							!= Flash::STATUS_t::SUCCESS) {
						reply = Boot::COMMAND_t::ERROR;
						break;
					}

					address.word.word_32 = address.word.word_32 + 8;

				}

				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_DONE: {
				reception_finished = 1;
				break;
			}
			}
			uint8_t res[1] = { reply };
			usb.sendData(res, 1);
		}

	}

	this->setState(Boot::STATE_t::UPDATE_VERIFY);
	if (this->validateNewFirmware() == Boot::STATUS_t::FAIL) {
		return Boot::STATUS_t::FAIL;
	}

	this->setState(Boot::STATE_t::UPDATE_INSTALLATION);
	if (this->finishUpdate() == Boot::STATUS_t::FAIL) {
		return Boot::STATUS_t::FAIL;

	}

	address.word.word_32 = UPDATE_REQUEST_ADDRESS;
	f.flashErase(address);
	DATA_t word;
	word.word.word_64 = 0x0;
	Storage s;
	s.writeData(address,word);



	BSP_LED_Off(LED9);
	BSP_LED_DeInit(LED9);
	return Boot::STATUS_t::SUCCESS;

}

Boot::STATUS_t Boot::validateNewFirmware(void) {

	if (this->checkNewFirmwareVersion() != Boot::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}
	if (this->checkNewFirmwareSignature() != Boot::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	return Boot::STATUS_t::SUCCESS;

}

Boot::STATUS_t Boot::finishUpdate(void) {

	Storage s;
	Flash f;
	DATA_t address;

	uint8_t new_fw_size[8];

	if (s.readData(new_fw_size, Storage::ASSET_t::NEW_FIRMWARE_SIZE)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	uint32_t new_fw_len = (new_fw_size[4] << 24) | (new_fw_size[5] << 16)
					| (new_fw_size[6] << 8) | new_fw_size[7];

	uint8_t new_fw[new_fw_len];

	address.word.word_32 = NEW_APP_START_ADDRESS;
	f.flashReadAll(address, new_fw, new_fw_len);

	address.word.word_32 = APP_START_ADDRESS;
	f.flashErase(address, 63);

	for (int i = 0; i < new_fw_len; i += 8) { // verificar divisibilidade por 8
		DATA_t word;

		word.word.word_64 = ((uint64_t) new_fw[i + 7] << 56)
						| ((uint64_t) new_fw[i + 6] << 48)
						| ((uint64_t) new_fw[i + 5] << 40)
						| ((uint64_t) new_fw[i + 4] << 32)
						| ((uint64_t) new_fw[i + 3] << 24)
						| ((uint64_t) new_fw[i + 2] << 16)
						| ((uint64_t) new_fw[i + 1] << 8) | ((uint64_t) new_fw[i] << 0);

		if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;

	}

	uint8_t hash[32];
	uint8_t assinatura[256];
	uint8_t version[8];

	if (s.readData(hash, Storage::ASSET_t::NEW_FIRMWARE_HASH)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	if (s.readData(assinatura, Storage::ASSET_t::NEW_FIRMWARE_SIGNATURE)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	if (s.readData(version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	address.word.word_32 = FIRMWARE_HASH_ADDRESS;
	f.flashErase(address);

	for (int i = 0; i < 32; i += 8) {
		DATA_t word;
		word.word.word_64 = ((uint64_t) hash[i + 7] << 56)
						| ((uint64_t) hash[i + 6] << 48)
						| ((uint64_t) hash[i + 5] << 40)
						| ((uint64_t) hash[i + 4] << 32)
						| ((uint64_t) hash[i + 3] << 24)
						| ((uint64_t) hash[i + 2] << 16) | ((uint64_t) hash[i + 1] << 8)
						| ((uint64_t) hash[i] << 0);

		if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;

	}

	address.word.word_32 = FIRMWARE_SIGNATURE_ADDRESS;

	for (int i = 0; i < 256; i += 8) {
		DATA_t word;
		word.word.word_64 = ((uint64_t) assinatura[i + 7] << 56)
						| ((uint64_t) assinatura[i + 6] << 48)
						| ((uint64_t) assinatura[i + 5] << 40)
						| ((uint64_t) assinatura[i + 4] << 32)
						| ((uint64_t) assinatura[i + 3] << 24)
						| ((uint64_t) assinatura[i + 2] << 16)
						| ((uint64_t) assinatura[i + 1] << 8)
						| ((uint64_t) assinatura[i] << 0);

		if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;
	}

	address.word.word_32 = FIRMWARE_SIZE_ADDRESS;

	DATA_t word;
	word.word.word_64 = ((uint64_t) new_fw_size[7] << 56)
					| ((uint64_t) new_fw_size[6] << 48)
					| ((uint64_t) new_fw_size[5] << 40)
					| ((uint64_t) new_fw_size[4] << 32);

	if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	address.word.word_32 = FIRMWARE_VERSION_ADDRESS;

	word.word.word_64 = ((uint64_t) version[7] << 56);

	if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	address.word.word_32 = NEW_APP_START_ADDRESS;
	if (f.flashErase(address, 63) != Flash::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;
	if (f.flashErase(address) != Flash::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	return Boot::STATUS_t::SUCCESS;
}

Boot::STATUS_t Boot::checkNewFirmwareVersion(void) {
	uint8_t fw_version[8];
	uint8_t new_fw_version[8];

	Storage s;

	if (s.readData(fw_version, Storage::ASSET_t::FIRMWARE_VERSION)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	if (s.readData(new_fw_version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	if (fw_version[7] < new_fw_version[7]) {
		return Boot::STATUS_t::SUCCESS;
	}

	return Boot::STATUS_t::FAIL;

}

Boot::STATUS_t Boot::checkFirmwareIntegrity(void) {

	CryptoRSA c;
	Flash f;

	uint8_t fw_size[8];
	Storage s;
	if (s.readData(fw_size, Storage::ASSET_t::FIRMWARE_SIZE)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	uint32_t fw_len = (fw_size[4] << 24) + (fw_size[5] << 16)
					+ (fw_size[6] << 8) + fw_size[7];

	uint8_t buffer_firmware[fw_len];

	DATA_t address;
	address.word.word_32 = APP_START_ADDRESS;

	f.flashReadAll(address, buffer_firmware, fw_len);

	uint8_t digest[32];
	size_t digest_len;

	CryptoRSA::STATUS_t status = CryptoRSA::STATUS_t::FAIL;
	status = c.shaGen(buffer_firmware, sizeof(buffer_firmware), digest,
			&digest_len, CryptoRSA::SHA_t::SHA256);

	if (status == CryptoRSA::STATUS_t::SUCCESS) {
		uint8_t hash_firmware[32];

		Storage s;
		s.readData(hash_firmware, Storage::ASSET_t::FIRMWARE_HASH);

		status = c.shaCheck(hash_firmware, digest, CryptoRSA::SHA_t::SHA256);
		if (status == CryptoRSA::STATUS_t::SUCCESS) {
			this->setIsReadyToBoot(Boot::STATUS_t::SUCCESS);
			return Boot::STATUS_t::SUCCESS;

		} else {
			return Boot::STATUS_t::FAIL;
		}

	} else {
		return Boot::STATUS_t::FAIL;
	}

	/* should never get here*/
	return Boot::STATUS_t::FAIL;

}

Boot::STATUS_t Boot::checkNewFirmwareSignature(void) {

	Boot::STATUS_t result = Boot::STATUS_t::FAIL;
	CryptoRSA c;
	Flash f;
	Storage s;
	uint8_t fw_size[8];

	if (s.readData(fw_size, Storage::ASSET_t::NEW_FIRMWARE_SIZE)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	uint32_t fw_len = (fw_size[4] << 24) + (fw_size[5] << 16)
					+ (fw_size[6] << 8) + fw_size[7];
	uint8_t buffer_firmware[fw_len+8];
	memset((void*) buffer_firmware, '\0', fw_len+8);
	uint8_t modulo[256];
	uint8_t expoente[3];

	if (s.readData(modulo, Storage::ASSET_t::PUBLIC_MODULUS)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	if (s.readData(expoente, Storage::ASSET_t::KEY_EXPONENT)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	CryptoRSA::chave_t chave_publica;
	chave_publica.expoente = (uint8_t*) expoente;
	chave_publica.modulo = (uint8_t*) modulo;
	chave_publica.expoente_len = 3;
	chave_publica.modulo_len = 256;
	uint8_t assinatura[256];

	if (s.readData(assinatura, Storage::ASSET_t::NEW_FIRMWARE_SIGNATURE)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	DATA_t address;
	address.word.word_32 = NEW_APP_START_ADDRESS;
	f.flashReadAll(address, buffer_firmware, fw_len);

	uint8_t new_fw_version[8];

	if (s.readData(new_fw_version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;

	}

	for (int i = fw_len; i < fw_len + 8; i++){
		buffer_firmware[i] = new_fw_version[i-fw_len];
	}



	if (c.sigCheck(assinatura, buffer_firmware, fw_len+8, &chave_publica,
			CryptoRSA::ALGORITHM_t::RSA_PKCS1_V1_5)
			== CryptoRSA::STATUS_t::SUCCESS) {
		result = Boot::STATUS_t::SUCCESS;
		return result;
	}

	/* should never get here*/
	return Boot::STATUS_t::FAIL;

}

