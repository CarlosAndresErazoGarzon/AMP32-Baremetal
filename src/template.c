#include "apm32f10x.h"
#include "delay.h"

/* ==============================================================================
   DEFINICIONES DE REGISTROS (MÁSCARAS BITWISE)
   ============================================================================== */

// --- ZONA 1: GESTIÓN DE RELOJES (RCM) ---
// APB2: Periféricos de Alta Velocidad (GPIO, ADC, USART1, SPI1)
#define RCM_APB2_AFIO   (1 << 0)  // Funciones Alternativas
#define RCM_APB2_GPIOA  (1 << 2)  // Puerto A
#define RCM_APB2_GPIOB  (1 << 3)  // Puerto B 
#define RCM_APB2_GPIOC  (1 << 4)  // Puerto C
#define RCM_APB2_ADC1   (1 << 9)  // Conversor Analógico-Digital 1
#define RCM_APB2_USART1 (1 << 14) // UART 1

// APB1: Periféricos de Baja Velocidad (TIM2/3/4, USART2/3, I2C)
#define RCM_APB1_TIM2   (1 << 0)  // Timer 2 (General Purpose)
#define RCM_APB1_USART2 (1 << 17) // UART 2

// --- ZONA 2: CONFIGURACIÓN GPIO (CRL/CRH) ---
// Modos: Input (00), Output 10MHz (01), Output 2MHz (10), Output 50MHz (11)
// Configs (Salida): Push-Pull (00), Open-Drain (01), AF Push-Pull (10), AF Open-Drain (11)
// Configs (Entrada): Analog (00), Floating (01), Pull-up/down (10)

// LED (PB2) -> Salida 50MHz, Push-Pull
#define GPIOB_MODE2_50MHZ (3 << 8)   
#define GPIOB_CNF2_PP     (0 << 10)

/* // UART1 TX (PA9) -> Salida 50MHz, Alt Function Push-Pull
#define GPIOA_MODE9_50MHZ (3 << 4)   // CRH (Pines 8-15) - Bit 4 empieza PA9
#define GPIOA_CNF9_AFPP   (2 << 6)   
// UART1 RX (PA10) -> Entrada Flotante
#define GPIOA_MODE10_IN   (0 << 8)
#define GPIOA_CNF10_FLOAT (1 << 10)
*/

int main(void) {
    /* INICIALIZACIÓN DEL SISTEMA */
    SystemInit();      // Configura el PLL a 72MHz o 96MHz según chip
    SysTick_Init();    // Inicia el contador de milisegundos

    /* ==========================================================================
       ACTIVACIÓN DE RELOJES (RCM)
       ========================================================================== */
    
    // Habilitar reloj para GPIOB (LED)
    RCM->APB2CLKEN |= RCM_APB2_GPIOB;

    /* Habilitar reloj para UART Debug y GPIOA
    RCM->APB2CLKEN |= (RCM_APB2_GPIOA | RCM_APB2_USART1);
    */

    /* Habilitar reloj para ADC (Sensores)
    RCM->APB2CLKEN |= RCM_APB2_ADC1;
    */

    /* ==========================================================================
       CONFIGURACIÓN GPIO
       ========================================================================== */

    // --- CONFIGURACIÓN LED (PB2) ---
    // Limpiar configuración actual de PB2 (Bits 8-11 en CFGLOW)
    GPIOB->CFGLOW &= ~(0xF << 8); 
    // Aplicar modo Salida 50MHz | Push-Pull
    GPIOB->CFGLOW |= (GPIOB_MODE2_50MHZ | GPIOB_CNF2_PP);

    /* CONFIGURACIÓN UART1 (PA9=TX, PA10=RX)
    // PA9 (TX): Alt Func Push-Pull 50MHz
    GPIOA->CFGHIGH &= ~(0xF << 4); // Limpiar bits 4-7 (PA9)
    GPIOA->CFGHIGH |= (GPIOA_MODE9_50MHZ | GPIOA_CNF9_AFPP);
    
    // PA10 (RX): Input Floating (Default reset state, pero explícito es mejor)
    GPIOA->CFGHIGH &= ~(0xF << 8); // Limpiar bits 8-11 (PA10)
    GPIOA->CFGHIGH |= (GPIOA_MODE10_IN | GPIOA_CNF10_FLOAT);
    */

    /* ==========================================================================
       CONFIGURACIÓN PERIFÉRICOS (UART, TIMERS, ADC)
       ========================================================================== */

    /* INIT UART1 (115200 baudios @ 72MHz PCLK2)
    // Baud Rate Calculation: 72,000,000 / (16 * 115200) = 39.0625
    // Mantissa = 39 (0x27), Fraction = 0.0625 * 16 = 1 (0x1) -> BRR = 0x271
    USART1->BRR = 0x271; 
    USART1->CTRL1 |= (1 << 13); // UE: USART Enable
    USART1->CTRL1 |= (1 << 3);  // TE: Transmitter Enable
    USART1->CTRL1 |= (1 << 2);  // RE: Receiver Enable
    */

    /* ==========================================================================
       BUCLE PRINCIPAL (SUPERLOOP)
       ========================================================================== */
    while(1) {
        
        // --- HEARTBEAT ---
        GPIOB->BC = (1 << 2);   
        delay_ms(1000);         
        
        GPIOB->BSC = (1 << 2);  
        delay_ms(1000);        

        /* --- LECTURA DE SENSORES (ADC) --- 
        */

        /* --- LÓGICA DE CONTROL --- 
        */

        /* --- TELEMETRÍA (UART) --- 
        // Enviar datos por serial para debug
        */
    }
    return 0;
}