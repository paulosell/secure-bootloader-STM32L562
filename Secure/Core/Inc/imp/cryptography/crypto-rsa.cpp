/*
 * crypto-rsa.cpp
 *
 *  Created on: 23 de fev de 2021
 *      Author: sellp
 */

#include "crypto-rsa.h"
#include <string.h>

uint8_t preallocated_buffer[4096];

CryptoRSA::CryptoRSA() {

}
CryptoRSA::STATUS_t CryptoRSA::shaGen(uint8_t *buffer_in, size_t in_len,
		uint8_t *buffer_out, size_t *out_len, SHA_t sha_type) {

	SHA256ctx_stt P_pSHA256ctx;
	uint32_t error_status = HASH_SUCCESS;

	/* Set the size of the desired hash digest */
	P_pSHA256ctx.mTagSize = CRL_SHA256_SIZE;

	/* Set flag field to default value */
	P_pSHA256ctx.mFlags = E_HASH_DEFAULT;

	error_status = SHA256_Init(&P_pSHA256ctx);

	/* check for initialization errors */
	if (error_status == HASH_SUCCESS) {
		/* Add data to be hashed */
		error_status = SHA256_Append(&P_pSHA256ctx, buffer_in,
				(int32_t) in_len);

		if (error_status == HASH_SUCCESS) {
			/* retrieve */
			error_status = SHA256_Finish(&P_pSHA256ctx, buffer_out,
					(int32_t*) out_len);
		}
	}

	if (error_status != HASH_SUCCESS) {
		return STATUS_t::FAIL;
	}
	return STATUS_t::SUCCESS;
}

CryptoRSA::STATUS_t CryptoRSA::sigCheck(uint8_t *buffer_expected,
		uint8_t *buffer_in, size_t buffer_in_len, key_t *pubkey, ALGORITHM_t algo) {

	membuf_stt mb_st;
	mb_st.mSize = sizeof(preallocated_buffer);
	mb_st.mUsed = 0;
	mb_st.pmBuf = preallocated_buffer;
	uint8_t digest[32];
	size_t digestLen = 0;
	CryptoRSA::STATUS_t status = this->shaGen(buffer_in,
			(int32_t) buffer_in_len, digest, &digestLen,
			CryptoRSA::SHA_t::SHA256);
	RSApubKey_stt chave;

	chave.mExponentSize = (int32_t) pubkey->exponent_len;
	chave.mModulusSize = (int32_t) pubkey->modulus_len;
	chave.pmExponent = pubkey->exponent;
	chave.pmModulus = pubkey->modulus;

	int32_t ret;
	if (status == CryptoRSA::STATUS_t::SUCCESS) {
		/* Sign with RSA */
		ret = RSA_PKCS1v15_Verify(&chave, digest, E_SHA256, buffer_expected,
				&mb_st);
	}
	if (ret != AUTHENTICATION_SUCCESSFUL) {
		return STATUS_t::FAIL;
	}
	return STATUS_t::SUCCESS;
}

