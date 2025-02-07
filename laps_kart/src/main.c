#include "stm32f4xx.h"
#include "usart.h"


void internal_clock();
void blink_led();
void enable_pwm_read();

void blink_led(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0; // output 
    GPIOA->ODR |= GPIO_ODR_OD5;
}

void enable_pwm_read(){  // Using TIM2 CH1 and PA0
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOA->MODER |= GPIO_MODER_MODER0_1; // alternative function
    GPIOA->AFR[0] |= (1 << (0 * 4));

    TIM2->PSC = 90-1;

    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;   // CC1 channel as input, IC1 mapped to TI1
    TIM2->CCER |= TIM_CCER_CC1E;       // Capture rising edges
    TIM2->CCER |= TIM_CCER_CC1P;       // Capture falling edges
    TIM2->CCER |= TIM_CCER_CC1NP;

    TIM2->DIER |= TIM_DIER_CC1IE;
    NVIC_EnableIRQ(TIM2_IRQn);

    TIM2->CR1 |= TIM_CR1_CEN;
}

#define APB1CLOCK 9000000000
#define TIM2PSC 90

float width = 0;
float freq = 0;

int is_first_captured = 0;
uint32_t rising = 0;
uint32_t falling = 0;
uint32_t difference = 0;


void TIM2_IRQHandler(void) {
   

    if (TIM2->SR & TIM_SR_CC1IF) { // check if interrupt
        if(is_first_captured == 0){
            rising = TIM2->CCR1;
            is_first_captured = 1;
        }

        else{
            falling = TIM2->CCR1;

            if(falling > rising){
                difference = falling - rising;
            }

            else if(rising > falling){
                difference = (0xffffffff - rising) + falling;
            }
            
            float refClock = APB1CLOCK/TIM2PSC;
            float mFactor = 1000000/refClock;

            width = difference * mFactor;
            freq = refClock/difference;

            TIM2->CNT = 0;
            is_first_captured = 0;
            //width = (width - 14580) * 100;
        }
    }

}

int main(void){
    internal_clock();
    blink_led();
    enable_pwm_read();

    while(1){
        usart_send_string("Hello, USART!\n");
        for (volatile int i = 0; i < 1000000; i++); 
    }

}
