#include <stdint.h>
#include <stm32f031x6.h>

void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void enablePullUp(GPIO_TypeDef *Port,uint32_t BitNumber);
void delay(volatile uint32_t dly);

void RedOn(void); // turns on the red LED without changing the others
void RedOff(void); // turns off the red LED without changing the others
void YellowOn(void); // turns on the yellow LED without changing the others
void YellowOff(void); // turns off the yellow LED without changing the others
void GreenOn(void); // turns on the green LED without changing the others
void GreenOff(void); // turns off the green LED without changing the others

//int ButtonPressed(void); // returns if the button is pressed. 0 otherwise

int main()
{
	uint32_t count = 0;
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	pinMode(GPIOA,9,1); // Make GPIOA bit 0 an output
	pinMode(GPIOA,10,1); // Make GPIOA bit 1 an output
	pinMode(GPIOA,12,1); // Make GPIOA bit 2 an output
	GPIOA -> ODR |= (1<<9);
	GPIOA -> ODR |= (1<<10);
	GPIOA -> ODR |= (1<<12);
}
