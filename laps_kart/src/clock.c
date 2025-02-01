#include "stm32f4xx.h"

void internal_clock() {
    RCC->CR |= RCC_CR_HSION;  // Enable HSI
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);  // Wait until HSI is ready

    RCC->CR &= ~RCC_CR_HSEON;
    
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;
    
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV4;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    /* Reset PLL settings */
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ);

    /* Set PLL source to HSI, set M, N, P, and Q values */
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSI | (8 << RCC_PLLCFGR_PLLM_Pos) | (180 << RCC_PLLCFGR_PLLN_Pos) | (0b00 << RCC_PLLCFGR_PLLP_Pos) | (4 << RCC_PLLCFGR_PLLQ_Pos));
    
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);  // Wait until PLL is ready

    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    }
