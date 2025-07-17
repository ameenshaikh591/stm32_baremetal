/*
 * gpio.h
 *
 *  Created on: Jul 9, 2025
 *      Author: E0836126
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#include <stdint.h>

	typedef struct {
		volatile uint32_t MODER;
		volatile uint32_t OTYPER;
		volatile uint32_t OSPEEDR;
		volatile uint32_t PUPDR;
		volatile uint32_t IDR;
		volatile uint32_t ODR;
		volatile uint32_t BSRR;
		volatile uint32_t LCKR;
		volatile uint32_t AFRL;
		volatile uint32_t AFRH;
	} GPIO_TypeDef;


	typedef enum {
		MODER,
		ODR,
		AFRL,
		AFRH,
	} register_t;

	typedef enum {
		PORT_A,
		PORT_B,
		PORT_C,
		PORT_D,
	} port_t;


	typedef enum {
		PIN_0,
		PIN_1,
		PIN_2,
		PIN_3,
		PIN_4,
		PIN_5,
		PIN_6,
		PIN_7,
		PIN_8,
		PIN_9,
		PIN_10,
		PIN_11,
		PIN_12,
		PIN_13,
		PIN_14,
		PIN_15,
	} pin_t;

	typedef enum {
		AF_0,
		AF_1,
		AF_2,
		AF_3,
		AF_4,
		AF_5,
		AF_6,
		AF_7,
		AF_8,
		AF_9,
		AF_10,
		AF_11,
		AF_12,
		AF_13,
		AF_14,
		AF_15,
	} alt_function_t;

	typedef enum {
		HIGH,
		LOW,
	} output_t;

	typedef enum {
		INPUT,
		GENERAL_PURPOSE,
		ALT_FUNCTION,
		ANALOG,
		DEFAULT,
	} mode_t;


class GPIO {
public:

	GPIO(port_t port);

	void enable();
	void disable();
	void pin_setup(pin_t pin, mode_t mode);
	void pin_setup(pin_t pin, mode_t mode, alt_function_t alt_function);
	void pin_output(pin_t pin, output_t output);
	void pin_reset(pin_t pin);
	void pin_toggle_output(pin_t pin);

private:

	uint32_t set_mask(pin_t pin, mode_t mode);
	uint32_t set_mask(pin_t pin, output_t output);
	uint32_t set_mask(pin_t pin, alt_function_t alt_function);

	GPIO_TypeDef* gpio_port_;
	port_t port_;

};

#endif /* SRC_GPIO_H_ */
