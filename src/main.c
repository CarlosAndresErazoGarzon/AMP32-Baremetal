#include "apm32f10x.h"
#include "delay.h"

// RCM (Reset Clock Management)
#define RCM_APB2CLKEN_PBEN (1 << 3)  // Bit 3 del registro APB2CLKEN activa GPIOB

// GPIO Configuration Low (CFGLOW)
// PB2 está en los bits 8-11 del registro CFGLOW/CRL
#define GPIOB_CFGLOW_MODE2 (3 << 8)  // Bits 8-9: Modo Salida 50MHz (11 binario = 3 decimal)
#define GPIOB_CFGLOW_CNF2  (0 << 10) // Bits 10-11: Salida Push-Pull (00 binario = 0 decimal)

int main(void) {
    // Inicialización del Sistema
    SystemInit();      // Relojes del sistema 
    SysTick_Init();    // Timer de milisegundos 

    // Configuración del GPIO PB2
    
    // Habilitar el reloj para el periférico GPIOB
    RCM->APB2CLKEN |= RCM_APB2CLKEN_PBEN;

    // Limpiar
    GPIOB->CFGLOW &= ~(0xF << 8);

    // Salida 50MHz, Push-Pull
    GPIOB->CFGLOW |= GPIOB_CFGLOW_MODE2; 

    while(1) {
        // Encender LED
        // BC = Bit Clear (Pone a 0 / GND) -> Si el LED va a VCC, esto lo ENCIENDE
        GPIOB->BC = (1 << 2);   
        delay_ms(1000);         
        
        // Apagar LED
        // BSC = Bit Set/Clear (Aquí escribe 1 / 3.3V) -> Esto lo APAGA
        GPIOB->BSC = (1 << 2);  
        delay_ms(1000);        
    }
    return 0;
}