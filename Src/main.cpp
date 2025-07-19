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
#include "CLI/CLI.h"

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

	CLI my_cli = CLI();
	my_cli.run();

	while (1) {

	}
}








