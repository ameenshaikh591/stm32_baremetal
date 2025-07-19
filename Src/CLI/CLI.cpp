/*
 * CLI.cpp
 *
 *  Created on: Jul 17, 2025
 *      Author: Ameen Shaikh
 */


#include "../uart.h"
#include "Manager.h"
#include "CLI.h"


CLI::CLI() {

}

void CLI::run() {
	UART *uart_test_obj = new UART(usart_num_t::USART2);
	uart_test_obj->enable();

	Manager *my_manager = new Manager();
	manager = my_manager;
	my_manager->connect_to_usart(uart_test_obj);
}


