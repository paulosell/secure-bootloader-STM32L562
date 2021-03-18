/*
 * usb.h
 *
 *  Created on: Feb 22, 2021
 *      Author: sellp
 */

#ifndef INC_IMP_COMM_USB_H_
#define INC_IMP_COMM_USB_H_

#include <iostream>
#include <string.h>
#include "interfaces/comm/comm.h"
#include "usb/App/usb_device.h"
#include "usb/App/usbd_cdc_if.h"

class USBCDC : public Comm{
public:
	USBCDC();
	~USBCDC(){
		deinit();
	}
	size_t sendData(uint8_t *buffer_out, size_t buffer_len);
	size_t getData(uint8_t *buffer_in, size_t *  buffer_len);
	bool hasData(void);

	/*
	 * User might need to add specific platform methods
	 */

	void clearBufferUSB(void);

private:
	uint8_t *buffer;
	bool has_data;
};


#endif /* INC_IMP_COMM_USB_H_ */
