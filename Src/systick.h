#ifndef SRC_SYSTICK_H_
#define SRC_SYSTICK_H_

#include <stdint.h>

typedef void (*subtimer_callback_t)();

struct Subtimer {
    uint32_t duration_ms;
    uint32_t counter;
    subtimer_callback_t handler;
    bool active;
    bool one_shot;
};

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

class Systick {
public:
    void configure_timer(int tick_duration_ms = 1);
    void enable();
    void disable();

    static void create_subtimer(uint32_t duration_ms, subtimer_callback_t handler, bool one_shot = false);
    static void tick_all();
    static uint32_t millis();
    static void delay(uint32_t ms);

private:
    static constexpr int MAX_SUBTIMERS = 8;
    static Subtimer subtimers[MAX_SUBTIMERS];
};

#endif
