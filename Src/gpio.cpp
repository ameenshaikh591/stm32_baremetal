#include "gpio.h"
#include <stdint.h>

#define GPIOA ((GPIO_TypeDef*) 0x40020000)
#define RCC_AHB1ENR (*((volatile uint32_t*) 0x40023830))
#define RCC_GPIOA_EN 1

GPIO::GPIO(port_t port) {
	switch (port) {
		case PORT_A:
			gpio_port_ = GPIOA;
			port_ = PORT_A;
			break;
		default:
			break;
	}
}

void GPIO::enable() {
	RCC_AHB1ENR |= RCC_GPIOA_EN;
}

void GPIO::disable() {
	RCC_AHB1ENR &= ~RCC_GPIOA_EN;
}

void GPIO::pin_setup(pin_t pin, mode_t mode) {
	gpio_port_->MODER |= set_mask(pin, mode);
}

void GPIO::pin_setup(pin_t pin, mode_t mode, alt_function_t alt_function) {
	gpio_port_->MODER |= set_mask(pin, mode);
	if (pin < 8) {
		gpio_port_->AFRL &= ~set_mask(pin, AF_15);
		gpio_port_->AFRL |= set_mask(pin, alt_function);
	} else {
		gpio_port_->AFRH &= ~set_mask(pin, AF_15);
		gpio_port_->AFRH |= set_mask(pin, alt_function);
	}
}

void GPIO::pin_output(pin_t pin, output_t output) {
	switch (output) {
		case HIGH:
			gpio_port_->ODR |= set_mask(pin, output);
			break;
		case LOW:
			gpio_port_->ODR &= ~set_mask(pin, output);
			break;
		default:
			break;
	}
}

void GPIO::pin_reset(pin_t pin) {
	gpio_port_->MODER &= ~set_mask(pin, DEFAULT);
	gpio_port_->ODR &= ~set_mask(pin, HIGH);
	if (pin < 8) {
		gpio_port_->AFRL &= ~set_mask(pin, AF_15);
	} else {
		gpio_port_->AFRH &= ~set_mask(pin, AF_15);
	}
}

void GPIO::pin_toggle_output(pin_t pin) {
	gpio_port_->ODR ^= set_mask(pin, HIGH);
}

uint32_t GPIO::set_mask(pin_t pin, mode_t mode) {
	int shift_amount = pin * 2;
	switch (mode) {
		case INPUT:
			return 0;

		case GENERAL_PURPOSE:
			return 1 << shift_amount;

		case ALT_FUNCTION:
			return 2 << shift_amount;

		case ANALOG:
			return 3 << shift_amount;

		case DEFAULT:
			return 3 << shift_amount;
	}
}

uint32_t GPIO::set_mask(pin_t pin, output_t output) {
	int shift_amount = pin;
	return 1 << shift_amount;
}

uint32_t GPIO::set_mask(pin_t pin, alt_function_t alt_function) {
	int shift_amount = (pin % 8) * 4;
	return static_cast<uint32_t>(alt_function) << shift_amount;
}

