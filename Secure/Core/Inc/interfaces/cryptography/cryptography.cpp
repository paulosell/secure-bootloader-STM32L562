/*
 * crypto.cpp
 *
 *  Created on: 15 de fev de 2021
 *      Author: sellp
 */


#include "cryptography.h"
/* USER INCLUDE BEGIN HERE */

/* USER PRIVATE INCLUDE ENDS HERE*/


/* USER MIGHT INITILIALIZE CLASS DUE TO PLATFORM NEEDS */
Cryptography::Cryptography(){

}

/* MUST BE IMPLEMENTED BY USER*/
Cryptography::STATUS_t Cryptography::shaGen(uint8_t * buffer_in, size_t in_len, uint8_t * buffer_out, size_t *out_len, SHA_t sha_type){
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Cryptography::STATUS_t Cryptography::shaCheck(uint8_t * buffer_expected, uint8_t * buffer_digest, SHA_t sha_type){

int size;
STATUS_t result = STATUS_t::SUCCESS;

switch(sha_type){
	case SHA_t::SHA1:
		size = 20;
		break;
	case SHA_t::SHA224:
		size = 28;
		break;
	case SHA_t::SHA256:
		size = 32;
		break;
	case SHA_t::SHA512:
		size = 64;
		break;
}

	while (size--) {	
		if (*buffer_expected != *buffer_digest) {
			result = STATUS_t::FAIL;
		}

		buffer_expected++;
		buffer_digest++;
	}

	return result;
}

/* MUST BE IMPLEMENTED BY USER*/
Cryptography::STATUS_t Cryptography::sigGen(uint8_t * buffer_in, size_t in_len, uint8_t * buffer_out, size_t *out_len, chave_t * privkey, ALGORITHM_t algo){
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Cryptography::STATUS_t Cryptography::sigCheck(uint8_t* buffer_expected, uint8_t * buffer_signature, size_t buffer_in_len, chave_t * pubkey, ALGORITHM_t algo){
	return STATUS_t::SUCCESS;
}
