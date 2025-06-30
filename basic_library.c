/*
 * basic_library.c
 *
 *  Created on: Jun 24, 2025
 *      Author: ameenshaikh
 */
#include "basic_library.h"













/*
 * setLD2()
 * The goal of this function is to turn on LD2
 * LD2 is connected to PA5 (GPIO Port A, Pin 5)
 *
 *
 * 1) 		We need to enable the clock for the GPIOA peripheral
 * 1.1) 	This is done by using the RCC AHB1 peripheral clock enable register
 * 1.2)		Setting the first bit of this register to "1" enables the GPIOA peripheral clock
 *
 * 2) 		Now, we need to set the mode of LD2's pin to "output" mode
 * 2.1)		We will use GPIOA_MODER (GPIOA Mode Register). Each pin has 2 bits for its mode
 * 2.2)		We will bit mask to wipe clean the bits corresponding to PA5
 * 2.3)		Then, we will assign "10" to PA5's bits. This sets PA5 to output mode
 *
 * 3.)		Now, we want to set PA5 to "high"
 * 3.1)		We will use GPIOA_ODR (GPIOA Output Data Register). Each pin has 1 bit for its output data
 * 3.2)		We will assign "1" to the 5th bit
 */

#define RCC_AHB1ENR   (volatile unsigned int*)0x40023830
#define GPIOA_MODER   (volatile unsigned int*)0x40020000
#define GPIOA_ODR     (volatile unsigned int*)0x40020014

void setLD2(int output) {
	// LD2 --> PA5
	// PA5 uses GPIOA, so we need to enable its clock
	// GPIOA's clock can be enabled by setting first bit of RCC AHB1 peripheral clock enable register
	*RCC_AHB1ENR = *RCC_AHB1ENR | 1;

	// Now, we need to set the mode of the pin, PA5, to "output" mode
	// We will use GPIOA's Mode Register. Each pin has 2 bits in this register
    *GPIOA_MODER &= ~(0b11 << (5 * 2));

    // We use bit masking to set the 10th and 11th bits to "01", which puts PA5 in output mode
    *GPIOA_MODER |=  (0b01 << (5 * 2));

    if (output) {
    	*GPIOA_ODR = *GPIOA_ODR | (1 << 5);
    } else {
    	*GPIOA_ODR = *GPIOA_ODR & ~(1 << 5);
    }

}



/*
 * systick_init()
 * SysTick is a 24-bit counting timer built into the ARM processor
 * When set, SysTick can decrement every rising edge of the processor clock
 * You can set SysTick to a particular level, where it will count down all the way to 0 and reset
 *
 *
 * 1)		We want to configure SysTick to our particular desires
 * 1.1)		SYST_CSR (SysTick Control and Status Register) helps us configure SysTick
 * 1.2)		We want to enable SysTick. Setting "1" to bit 0 does this
 * 1.3)		We want to use processor clock for SysTick. Setting "1" to bit 2 does this
 *
 * 2)		We want to determine what value SysTick should start decrementing from & reload to
 * 2.1)		SYST_RVR (SysTick Reload Value Register) helps us set the starting value
 * 2.2)		Since processor clock is 16MHz, it will take 1ms to decrement from 15,999 to 0
 *
 * 3)		We want to set the current value of SysTick
 * 3.1)		SYST_CVR (SysTick Current Value Register) helps us read/write the current value
 * 3.2)		By setting SYST_CVR to 0, we effectively reset SysTick, as it will reload to 15,999
 */

#define SYST_CSR   (*(volatile unsigned int*)0xE000E010)  // Control and status
#define SYST_RVR   (*(volatile unsigned int*)0xE000E014)  // Reload value
#define SYST_CVR   (*(volatile unsigned int*)0xE000E018)  // Current value

void systick_init(void) {
    SYST_RVR = 16000 - 1;         // 16,000 cycles @ 16 MHz = 1 ms
    SYST_CVR = 0;                 // Clear current value
    SYST_CSR = 0b111;             // Enable counter, interrupt, use processor clock
}

/*
 * delay_ms()
 * Used to set a delay in ms
 *
 * 1.)		We utilize SYST_CSR (SysTick Control & Status Register) to determine if 1ms has past
 * 1.1)		Bit 15 of SYST_CSR is "COUNTFLAG". "COUNTFLAG" goes to 1 whenever a reload has occurred
 * 1.2)		We constantly poll "COUNTFLAG". Thus, after ~1ms, "COUNTFLAG" will go to 1 and we detect it
 * 1.3)		Whenever "COUNTFLAG" is read from, it is cleared to 0
 */

void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms; i++) {
        while ((SYST_CSR & (1 << 16)) == 0);  // Wait until COUNTFLAG is set
    }
}



