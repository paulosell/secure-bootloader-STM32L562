/*
 * switcher.h
 *
 *  Created on: 17 de fev de 2021
 *      Author: sellp
 */

#ifndef INTERFACES_SWITCHER_SWITCHER_H_
#define INTERFACES_SWITCHER_SWITCHER_H_

#include <iostream>
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

class Switcher {
public:

	Switcher();
	virtual void resetDevice(void);
	virtual void goToSecure(void);
	virtual void goToNonSecure(void);
};

#endif /* INTERFACES_SWITCHER_SWITCHER_H_ */
