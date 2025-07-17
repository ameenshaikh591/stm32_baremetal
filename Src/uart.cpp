/*
 * uart.cpp
 *
 *  Created on: Jul 10, 2025
 *      Author: E0836126
 */


#include <stdint.h>
#include "uart.h"
#include "gpio.h"
#include <cstring>

// RCC register definitions
#define RCC_BASE       		(0x40023800UL)
#define RCC_APB1ENR    		(*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB1ENR_USART2 	(1 << 17)

// USART2 base address
#define USART2_BASE    		(0x40004400UL)
#define USART_2 			((usart_typedef *) USART2_BASE)

// NVIC ISER register
#define NVIC_ISER1			(*(volatile uint32_t*)(0xE000E104))
#define ISER1_USART2INT		(1 << 6)

UART* UART::usart_instances[6] = { nullptr };
static char echo_buf[2];

UART::UART(usart_num_t usart_num)
    : usart_num(usart_num), gpio_port(PORT_A)
{
    switch (usart_num) {
        case usart_num_t::USART2:
            usart_port_ = USART_2;
            break;
        default:
            break;
    }
}


void UART::enable() {
	switch (usart_num) {
		case usart_num_t::USART2:
			RCC_APB1ENR |= RCC_APB1ENR_USART2;
			configure_pins();
			configure_registers();
			usart_instances[1] = this;
			break;
		default:
			break;
	}

};

void UART::configure_pins() {
	gpio_port.enable();
	pin_t RX_PIN;
	pin_t TX_PIN;
	switch (usart_num) {
		case usart_num_t::USART2:
			RX_PIN = PIN_3;
			TX_PIN = PIN_2;
			break;
		default:
			break;
	}
	gpio_port.pin_setup(TX_PIN, ALT_FUNCTION, AF_7);
	gpio_port.pin_setup(RX_PIN, ALT_FUNCTION, AF_7);
}

#define USARTx_CR1_UE 		(1 << 13)
#define USARTx_CR2_TE 		(1 << 3)
#define USARTx_CR1_TXEIE    (1 << 7)
#define BAUD_9600 			0x0683
#define USARTx_CR1_RE 		(1 << 2)
#define USARTx_CR1_RXNEIE	(1 << 5)

void UART::configure_registers() {
	usart_port_->CR1 |= USARTx_CR1_UE; // USART peripheral enable
	usart_port_->BRR = BAUD_9600;
	usart_port_->CR1 |= USARTx_CR2_TE; // transmit enable
	usart_port_->CR1 |= USARTx_CR1_RE; // receive enable
	usart_port_->CR1 |= USARTx_CR1_RXNEIE;
	NVIC_ISER1 |= ISER1_USART2INT;
}

#define USARTx_SR_RXNE		(1 << 5)
#define USARTx_SR_TXE		(1 << 7)

extern "C" void USART2_IRQHandler(void) {
	int usart2_inst_index = 1;
	UART *usart2 = UART::usart_instances[usart2_inst_index];
	if (usart2->usart_port_->SR & USARTx_SR_RXNE) {
		UART::usart_instances[usart2_inst_index]->read_byte();
	}
	if (usart2->usart_port_->SR & USARTx_SR_TXE) {
		UART::usart_instances[usart2_inst_index]->send_byte_handler();
	}
}

void UART::send_byte_handler(void) {
	if (curr_pos < message_len - 1) {
		usart_port_->DR = message[curr_pos];
		curr_pos++;
	} else if (curr_pos == message_len - 1) {
		usart_port_->DR = message[curr_pos];
		usart_port_->CR1 &= ~USARTx_CR1_TXEIE;
	}
}

void UART::send(char *message) {
	this->message = message;
	this->message_len = strlen(message);
	curr_pos = 0;
	usart_port_->CR1 |= USARTx_CR1_TXEIE;
}

void UART::read_byte() {
	char character = static_cast<char>(usart_port_->DR);
	echo_buf[0] = character;
	echo_buf[1] = '\0';
	send(echo_buf);
}

inline bool UART::tx_ready() {
	return (usart_port_->SR & (1 << 7)) != 0;
}

/*
 * Configure the mask bits of the 23 interrupt lines (EXTI_IMR)
• Configure the Trigger selection bits of the interrupt lines (EXTI_RTSR and EXTI_FTSR)
• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
external interrupt controller (EXTI) so that an interrupt coming from one of the 23 lines
can be correctly acknowledged
 */


