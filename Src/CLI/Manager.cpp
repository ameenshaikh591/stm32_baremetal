/*
 * Manager.cpp
 *
 *  Created on: Jul 18, 2025
 *      Author: E0836126
 */

#include "../uart.h"
#include "Manager.h"
#include <stdint.h>

#define COMMAND_BUFFER_SIZE 100

enum class rec_buffer_t {
	LINEAR_BUFFER,
	RING_BUFFER,
};

static char command_buffer[COMMAND_BUFFER_SIZE];
static char echo_buffer[2];

void Manager::connect_to_usart(UART *usart_obj) {
	usart_obj->usart_manager = this;
	managed_usart = usart_obj;
	command_buffer[0] = '\r';
	command_buffer[1] = '\n';
}

void Manager::send_response(char *buffer_to_send) {
	managed_usart->send(buffer_to_send);
}

void Manager::manage_read_byte(uint32_t received_byte) {
	static uint8_t buffer_open_index = 2;
	char character = static_cast<char>(received_byte);
	if (buffer_open_index < COMMAND_BUFFER_SIZE - 3) {
		if (character == '\r') {
			command_buffer[buffer_open_index++] = '\r';
			command_buffer[buffer_open_index++] = '\n';
			command_buffer[buffer_open_index] = '\0';
			buffer_open_index = 2;
			send_response(command_buffer);
			return;
		} else {
			command_buffer[buffer_open_index++] = character;
		}
	} else {
	    buffer_open_index = 2;
	    command_buffer[buffer_open_index++] = character;
	}

	echo_buffer[0] = character;
	echo_buffer[1] = '\0';
	send_response(echo_buffer);
}


