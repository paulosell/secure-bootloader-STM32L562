/*
 * comm.cpp
 *
 *  Created on: 15 de fev de 2021
 *      Author: sellp
 */

#include "comm.h"

/* USER INCLUDE BEGIN HERE */

/* USER PRIVATE INCLUDE ENDS HERE*/

/* USER MUST INIT CLASS WITH OWN COMMUNICATION INTERFACE */
Comm::Comm(){
	this->has_data = false;
}


/* MUST BE IMPLEMENTED BY USER*/
size_t Comm::sendData(uint8_t *buffer_out, size_t buffer_len){
	return 0;
}

/* MUST BE IMPLEMENTED BY USER*/
size_t Comm::getData(uint8_t *buffer_in, size_t * buffer_len){
	return 0;
}

/* MUST BE IMPLEMENTED BY USER*/
bool Comm::hasData(void){
	/*
	 * Might need to add callback treatment
	 */

	return this->has_data;
}




