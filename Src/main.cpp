/*
 * main.cpp
 *
 *  Created on: Jul 9, 2025
 *      Author: E0836126
 */



#include <stdint.h>
#include "gpio.h"
#include "systick.h"
#include "uart.h"

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


/*
 * 	GPIOA.enable();
	GPIOA.pin_setup(PIN_5, GENERAL_PURPOSE);
	GPIOA.pin_output(PIN_5, HIGH);

	Systick SysTick;

	SysTick.configure_timer(1);
	SysTick.enable();
	SysTick.create_subtimer(500, blink_led);
 */





void uart_test() {
	Systick SysTick;
	SysTick.configure_timer();
	SysTick.enable();

	UART uart_test_obj = UART(usart_num_t::USART2);
	uart_test_obj.enable();
	uart_test_obj.send("\rHi\n");
	//SysTick.delay(500);
	//uart_test_obj.send("\rAmeen\n");

	while (1) {

	}
}








