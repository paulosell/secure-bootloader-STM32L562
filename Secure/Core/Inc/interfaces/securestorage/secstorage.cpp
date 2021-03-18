/*
 * securestorage.cpp
 *
 *  Created on: 16 de fev de 2021
 *      Author: sellp
 */

#include "secstorage.h"
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/


/* USER MIGHT INITILIALIZE CLASS DUE TO PLATFORM NEEDS */
SecureStorage::SecureStorage(){

}

/* MUST BE IMPLEMENTED BY USER*/
SecureStorage::STATUS_t SecureStorage::readData(DATA_t address, uint8_t * out, ASSET_t asset){
	return STATUS_t::SUCCESS;

}

/* MUST BE IMPLEMENTED BY USER*/
SecureStorage::STATUS_t SecureStorage::writeData(DATA_t address, DATA_t data){
	return STATUS_t::SUCCESS;
}


