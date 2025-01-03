#include "stm32f4xx.h"

void internal_clock() {
    /* Enable HSE (High-Speed External) oscillator */
    RCC->CR |= RCC_CR_HSEON;

    /* Wait till HSE is ready */
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    /* Configure the Flash prefetch and latency settings */
    FLASH->ACR |= FLASH_ACR_PRFTEN;   // Enable Prefetch Buffer
    FLASH->ACR |= FLASH_ACR_ICEN;    // Instruction Cache Enable
    FLASH->ACR |= FLASH_ACR_DCEN;    // Data Cache Enable
    FLASH->ACR |= FLASH_ACR_LATENCY_3WS; // Set 3 wait states (48 MHz or above)

    /* Configure PLL */
    // PLLM = 8, PLLN = 336, PLLP = 2, PLLQ = 7
    RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos)    // PLLM: Divide HSE by 8
                 | (336 << RCC_PLLCFGR_PLLN_Pos)  // PLLN: Multiply by 336
                 | (0 << RCC_PLLCFGR_PLLP_Pos)    // PLLP: Divide by 2
                 | (7 << RCC_PLLCFGR_PLLQ_Pos)    // PLLQ: Divide by 7
                 | RCC_PLLCFGR_PLLSRC_HSE;       // Select HSE as PLL source

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Configure AHB, APB1, and APB2 clocks */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    // AHB = SYSCLK
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;   // APB1 = SYSCLK / 4
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;   // APB2 = SYSCLK / 2

    /* Select PLL as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;          // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL;       // Set PLL as system clock

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
