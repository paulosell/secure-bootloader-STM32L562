/*
 * usb.cpp
 *
 *  Created on: Feb 22, 2021
 *      Author: sellp
 */

#include "usb.h"


USBCDC::USBCDC(){

	 MX_USB_Device_Init();
     HAL_Delay(1000);
}

size_t USBCDC::sendData(uint8_t *buffer_out, size_t buffer_len){
	HAL_Delay(1);
	uint8_t result;
	result = CDC_Transmit_FS(buffer_out, buffer_len);

	if(result == USBD_OK){
		return buffer_len;
	}else {
		return 0;
	}


}
size_t USBCDC::getData(uint8_t *buffer_in, size_t *  buffer_len){
	this->buffer = get_received_data();
	this->has_data = 0;
	//buffer_in = this->buffer;

	uint32_t len = (this->buffer[1] << 24) | (this->buffer[2] << 16)
													| (this->buffer[3] << 8) | this->buffer[4];
	memcpy((void*) buffer_in, (const void*) this->buffer, len+5);
	*buffer_len = len;
	return len;

}

bool USBCDC::hasData(void){
	this->has_data = get_has_data();

	return this->has_data;
}

void USBCDC::clearBufferUSB(void){
	clear_buffer();

}
