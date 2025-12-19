#include "delay.h"
#include "apm32f10x.h"

volatile uint32_t msTicks = 0;

void SysTick_Init(void) {
    // Configura SysTick para 1ms
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); // Error
    }
}

void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

void SysTick_Handler(void) {
    msTicks++;
}