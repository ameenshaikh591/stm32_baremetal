/*
 * Manager.h
 *
 *  Created on: Jul 18, 2025
 *      Author: E0836126
 */

#include "../uart.h"

#ifndef CLI_MANAGER_H_
#define CLI_MANAGER_H_

class UART;

class Manager {

public:
	void manage_read_byte(uint32_t received_byte);
	void send_response(char *buffer_to_send);
	void connect_to_usart(UART *usart_obj);
private:
	UART *managed_usart;
};



#endif /* CLI_MANAGER_H_ */
