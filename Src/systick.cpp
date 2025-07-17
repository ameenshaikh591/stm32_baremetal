#include "systick.h"

#define SYSTICK_CTRL_EN      (1 << 0)
#define SYSTICK_CTRL_INT     (1 << 1)
#define SYSTICK_CTRL_SYSCLK  (1 << 2)
#define SYSTICK ((SysTick_TypeDef *)0xE000E010UL)

volatile uint32_t system_tick_ms = 0;
Subtimer Systick::subtimers[MAX_SUBTIMERS];

extern "C" void SysTick_Handler(void) {
    system_tick_ms++;
    Systick::tick_all();
}

void Systick::configure_timer(int tick_duration_ms) {
    disable();
    SYSTICK->LOAD = (16000 * tick_duration_ms) - 1;  // assuming 16 MHz system clock
    SYSTICK->VAL = 0;
}

void Systick::enable() {
    SYSTICK->CTRL |= SYSTICK_CTRL_EN | SYSTICK_CTRL_INT | SYSTICK_CTRL_SYSCLK;
}

void Systick::disable() {
    SYSTICK->CTRL = 0;
}

void Systick::create_subtimer(uint32_t duration_ms, subtimer_callback_t handler, bool one_shot) {
    for (int i = 0; i < MAX_SUBTIMERS; ++i) {
        if (!subtimers[i].active) {
            subtimers[i] = { duration_ms, 0, handler, true, one_shot };
            return;
        }
    }
}

void Systick::tick_all() {
    for (int i = 0; i < MAX_SUBTIMERS; ++i) {
        if (subtimers[i].active) {
            subtimers[i].counter++;
            if (subtimers[i].counter >= subtimers[i].duration_ms) {
                subtimers[i].counter = 0;
                if (subtimers[i].one_shot) {
                    subtimers[i].active = false;
                }
                if (subtimers[i].handler) {
                    subtimers[i].handler();
                }
            }
        }
    }
}

uint32_t Systick::millis() {
    return system_tick_ms;
}

void Systick::delay(uint32_t ms) {
    uint32_t start = system_tick_ms;
    while ((system_tick_ms - start) < ms) {
        // busy-wait
    }
}





