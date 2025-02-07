#include "stm32f4xx.h"
#include "usart.h"

void usart_init(void) {
    // Enable clocks for GPIOA and USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 and PA3 for USART2 (AF7)
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
    GPIOA->AFR[0] |= (7 << (2 * 4)) | (7 << (3 * 4));

    // Configure USART2
    USART2->BRR = 0x0683; // 9600 baud rate @ 16 MHz
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable transmitter and receiver
    USART2->CR1 |= USART_CR1_UE; // Enable USART
}

void usart_send_char(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void usart_send_string(const char* str) {
    while (*str) {
        usart_send_char(*str++);
    }
}

int __io_putchar(int ch) {
    usart_send_char(ch);
    return ch;
}