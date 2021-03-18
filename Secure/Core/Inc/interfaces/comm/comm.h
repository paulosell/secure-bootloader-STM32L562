/*
 * comm.h
 *
 *  Created on: 15 de fev de 2021
 *      Author: sellp
 */

#ifndef COMM_COMM_H_
#define COMM_COMM_H_

#include <iostream>

/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

class Comm {

public:
	Comm();
	virtual size_t sendData(uint8_t *buffer_out, size_t buffer_len);
	virtual size_t getData(uint8_t *buffer_in, size_t * buffer_len);
	virtual bool hasData(void);

	/*
	 * User might need to add specific platform methods
	 */

private:
	uint8_t *buffer;
	bool has_data;
};

#endif /* COMM_COMM_H_ */
