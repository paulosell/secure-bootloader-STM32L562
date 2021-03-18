/*
 * bootloader.cpp
 *
 *  Created on: 17 de fev de 2021
 *      Author: sellp
 */

#include "bootloader.h"
#include "../comm/comm.h"
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

/* USER MIGHT INITILIALIZE CLASS DUE TO PLATFORM NEEDS */
Bootloader::Bootloader() {
	this->state = STATE_t::BOOTLOADER;
	this->is_ready_to_boot = STATUS_t::FAIL;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkFirmwareIntegrity(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkFirmwareSignature(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkFirmwareVersion(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkNewFirmwareIntegrity(void) {
	// read all flash space reserved for new firmware
	// do hash of new firmware
	// compare hash of new firmware with hash received

	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkNewFirmwareSignature(void) {
	// read all flash space reserved for new firmware
	// do compare signature of new firmware


	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkNewFirmwareVersion(void) {
	// read space where new firmware version is stored
	// compare new version vs old version
	return STATUS_t::SUCCESS;
}


Bootloader::STATUS_t Bootloader::hasToUpdate(void){

}



Bootloader::STATE_t Bootloader::getState(void) {
	return this->state;
}

Bootloader::STATUS_t Bootloader::setState(Bootloader::STATE_t state) {
	this->state = state;
	return STATUS_t::SUCCESS;
}

Bootloader::STATUS_t Bootloader::getIsReadyToBoot(void){
	return this->is_ready_to_boot;
}
Bootloader::STATUS_t Bootloader::setIsReadyToBoot(STATUS_t status){
	this->is_ready_to_boot = status;
	return STATUS_t::SUCCESS;
}


