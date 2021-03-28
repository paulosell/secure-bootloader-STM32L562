/*
 * secure.cpp
 *
 *  Created on: 2 de mar de 2021
 *      Author: sellp
 */

#include "storage.h"
#include "../flash/flash.h"
#include <string.h>

Storage::STATUS_t Storage::readData(uint8_t * out, ASSET_t asset){

	Storage::STATUS_t status = Storage::STATUS_t::SUCCESS;
	switch (asset){
	case Storage::ASSET_t::MODULUS:
		memcpy((void*) out, (const void*) MODULUS_ADDRESS, 256);
		break;
	case Storage::ASSET_t::PUBLIC_EXPONENT:
		memcpy((void*) out, (const void*)PUBLIC_EXPONENT_ADDRESS, 3);
		break;
	case Storage::ASSET_t::FIRMWARE_HASH:
		memcpy((void*) out, (const void*)FIRMWARE_HASH_ADDRESS, 32);
		break;
	case Storage::ASSET_t::FIRMWARE_SIGNATURE:
		memcpy((void*) out, (const void*) FIRMWARE_SIGNATURE_ADDRESS, 256);
		break;
	case Storage::ASSET_t::FIRMWARE_VERSION:
		memcpy((void*) out, (const void*) FIRMWARE_VERSION_ADDRESS, 8);
		break;
	case Storage::ASSET_t::NEW_FIRMWARE_HASH:
		memcpy((void*) out, (const void*) NEW_FIRMWARE_HASH_ADDRESS, 32);
		break;
	case Storage::ASSET_t::NEW_FIRMWARE_SIGNATURE:
		memcpy((void*) out, (const void*) NEW_FIRMWARE_SIGNATURE_ADDRESS, 256);
		break;
	case Storage::ASSET_t::NEW_FIRMWARE_VERSION:
		memcpy((void*) out, (const void*) NEW_FIRMWARE_VERSION_ADDRESS, 8);
		break;
	case Storage::ASSET_t::FIRMWARE_SIZE:
		memcpy((void*) out, (const void*) FIRMWARE_SIZE_ADDRESS, 8);
		break;
	case Storage::ASSET_t::NEW_FIRMWARE_SIZE:
		memcpy((void*) out, (const void*) NEW_FIRMWARE_SIZE_ADDRESS, 8);
		break;
	case Storage::ASSET_t::UPDATE_REQUEST:
		memcpy((void*) out, (const void*) UPDATE_REQUEST_ADDRESS, 8);
		break;
	default:
		status = Storage::STATUS_t::FAIL;
		break;
	}

	return status;


}

Storage::STATUS_t Storage::writeData(DATA_t address, DATA_t data){
	Flash f;

	if (f.flashWrite(address, data) != Flash::STATUS_t::SUCCESS){
		return Storage::STATUS_t::FAIL;
	}

	return Storage::STATUS_t::SUCCESS;




}
