/*
 * crypto-rsa.h
 *
 *  Created on: 23 de fev de 2021
 *      Author: sellp
 */

#ifndef INC_IMP_CRYPTOGRAPHY_CRYPTO_RSA_H_
#define INC_IMP_CRYPTOGRAPHY_CRYPTO_RSA_H_


#include <iostream>
#include "interfaces/cryptography/cryptography.h"
#include "crypto.h"



class CryptoRSA : public Cryptography {
public:

	CryptoRSA();
	STATUS_t shaGen(uint8_t * buffer_in, size_t in_len, uint8_t * buffer_out, size_t * out_len, SHA_t sha_type);
	STATUS_t sigGen(uint8_t * buffer_in, size_t in_len, uint8_t * buffer_out, size_t *out_len, chave_t * privkey, ALGORITHM_t algo);
	STATUS_t sigCheck(uint8_t* buffer_expected, uint8_t * buffer_signature, size_t buffer_in_len, chave_t * pubkey, ALGORITHM_t algo);

};


#endif /* INC_IMP_CRYPTOGRAPHY_CRYPTO_RSA_H_ */
