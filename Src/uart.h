#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "gpio.h"
#include "CLI/Manager.h"

// RCC register definitions
#define RCC_BASE       (0x40023800UL)
#define RCC_APB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB1ENR_USART2 (1 << 17)

// USART2 base address
#define USART2_BASE    (0x40004400UL)
#define USART2_SR      (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR      (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR     (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1     (*(volatile uint32_t *)(USART2_BASE + 0x0C))



typedef struct {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
} usart_typedef;

enum class usart_num_t {
	USART1,
	USART2,
	USART3,
	USART4,
	USART5,
	USART6,
};

enum class usart_rec_buffer_t {
	ECHO,
	LINEAR_BUFFER,
	RING_BUFFER,
};

class UART {
public:
	static UART* usart_instances[6];
    explicit UART(usart_num_t);
    void enable();
    void send(char *message);
    void read_byte();
    usart_typedef *usart_port_;
    void send_byte_handler();
    char* get_buffer_ptr();
    Manager *usart_manager;
    bool tx_in_progress = false;

private:
    const int baud_rate = 9600;
    const uint8_t num_data_bits = 8;
    const uint8_t num_stop_bits = 1;
    const uint8_t num_parity_bits = 0;
    GPIO gpio_port;
    usart_num_t usart_num;
    char *message;
    int message_len;
    int curr_pos;
    usart_rec_buffer_t rec_buffer_type;
    char *linear_buffer_ptr;
    uint8_t buffer_open_index;

    inline bool tx_ready();
    void configure_pins();
    void configure(usart_rec_buffer_t rec_buffer_type);
    void store_linear_buffer(char character);
};

#endif
/*
class UART::Transmitter {
public:
    explicit Transmitter(volatile uint32_t *data_reg, volatile uint32_t *status_reg);
    void send(uint8_t byte);
};

#endif /* UART_H_ */



// 1. USART2 is on the APB1 bus
// All peripheral clocks are disabled on reset, so I need to go to RCC_APB1ENR, bit 17
// Address of RCC_APB1ENR --> 0x40023840

// USART2's peripheral registers are in this address space: 0x40004400 - 0x400047FF
