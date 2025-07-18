/*
 * main.cpp
 *
 *  Created on: Jul 9, 2025
 *      Author: Ameen Shaikh
 */



#include <stdint.h>
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "CLI/Manager.h"

GPIO GPIOA = GPIO(PORT_A);

void blink_led() {
	static bool state = false;
	state = !state;
	GPIOA.pin_output(PIN_5, state ? HIGH : LOW);
}

void uart_test();
int main(void)
{
	uart_test();
}


void uart_test() {
	Systick SysTick;
	SysTick.configure_timer();
	SysTick.enable();

	UART uart_test_obj = UART(usart_num_t::USART2);
	uart_test_obj.enable();

	Manager my_manager = Manager();
	my_manager.connect_to_usart(&uart_test_obj);

	while (1) {

	}
}








