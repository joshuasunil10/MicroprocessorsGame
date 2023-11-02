#include <stdint.h>
#include <stm32f031x6.h>

void playNote(uint32_t Freq)
{	
    // Counter is running at 65536 Hz 
    TIM14->ARR = (uint32_t)65536 / ((uint32_t)Freq); 
    TIM14->CCR1 = TIM14->ARR / 2;	
    TIM14->CNT = 0; // set the count to zero initially
    TIM14->CR1 |= (1 << 0); // and enable the counter
}

void initSound()
{
    // Power up the timer module
    RCC->APB1ENR |= (1 << 8);

    // Configure PF0 as an alternate function for PWM output
    GPIOF->MODER &= ~(3U << (2 * 0)); // Clear the bits for PF0
    GPIOF->MODER |= (2U << (2 * 0)); // Set PF0 to alternate function mode
    GPIOF->AFR[0] &= ~(0xFU << (4 * 0)); // Clear the bits for AF0 (TIM14) on PF0

    TIM14->CR1 = 0; // Set Timer 14 to default values
    TIM14->CCMR1 = (1 << 6) + (1 << 5);
    TIM14->CCER |= (1 << 0);
    TIM14->PSC = 48000000UL / 65536UL; // Use the prescaler to set the counter running at 65536 Hz

    // Configure ARR and CCR1 based on desired note frequency
    // Assuming C4 (you may need to adjust the frequency accordingly)
    uint32_t desiredFrequency = 261.63; // C4 frequency in Hz
    TIM14->ARR = 48000000UL / (TIM14->PSC * desiredFrequency);
    TIM14->CCR1 = TIM14->ARR / 2;

    TIM14->CNT = 0;
}