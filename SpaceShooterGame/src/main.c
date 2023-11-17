/*
Space Shooter Game

Developed by Harry Shackleton & Joshua Sunil Mathew

Microporcessors Module Coursework - TU857/2

"A SIMPLE SPACE SHOOTER GAME WHERE A SPACESHIP AVOIDS/SHOOTS METEORS HEADED FOR IT"


*/

// INCLUDES
#include <stm32f031x6.h>
#include "display.h"
#include <stdio.h>
#include "musicalnotes.h"
#include "sound.h"
#include <stdlib.h>
#include "serial.h"

// FUNCTIONS
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void endgame(int x);
void printscore(int x);
void startgame();
void log(char log[]);
void theme();
void game();

volatile uint32_t milliseconds;

// ASSET GENERATION
const uint16_t virt[]=
{
0,0,0,0,0,0,0,0,0,27482,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,27482,27482,27482,3163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,27482,120,120,25037,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,50497,160,160,58101,42040,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,17993,168,168,58093,33856,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,9801,152,152,17066,34377,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,51986,26682,59209,128,128,17049,9793,27482,53018,0,0,0,0,0,0,0,0,0,0,0,46612,31005,27482,42569,176,176,41914,50769,27482,39197,46612,0,0,0,0,0,0,0,0,0,27482,19290,27482,62364,9801,176,176,33466,34121,62364,27482,27482,27482,0,0,0,0,0,0,0,27482,27482,62364,62364,62364,1609,128,128,24961,9545,62364,62364,62364,27482,27482,0,0,0,0,0,3171,62364,27482,62364,62364,29605,34361,61051,61051,46245,1593,62364,62364,62364,27482,62364,27482,0,0,0,27482,62364,62364,27482,3584,3584,3584,25897,793,793,49944,34089,3584,3584,3584,27482,62364,62364,27482,0,27482,62364,62364,62364,27482,3584,3584,3584,34064,9785,9785,51001,33800,61184,3584,3584,27482,62364,62364,62364,27482,2816,2816,2816,2816,2816,62364,62364,62364,1577,19290,19290,36203,50465,62364,62364,62364,2816,2816,2816,2816,2816,0,0,0,0,0,62364,62364,62364,26161,36996,36996,28804,50473,62364,62364,62364,0,0,0,0,0,0,0,0,0,0,50960,12544,61440,33544,10050,10050,59193,58112,45312,12288,26401,0,0,0,0,0,0,0,0,0,0,27755,28011,2114,29869,0,0,0,20355,11090,2642,44667,0,0,0,0,0,0,0,0,0,0,15982,53273,21010,24327,0,0,0,24327,53273,4113,24327,0,0,0,0,0,0,0,0,0,0,32327,15873,57092,24327,0,0,0,40527,24066,16131,24327,0,0,0,0,0,0,0,0,0,0,24327,32518,7703,24327,0,0,0,24327,24334,24367,24327,0,0,0,0,0,0,0,0,0,0,0,48503,24327,0,0,0,0,0,24327,24327,0,0,0,0,0,0,
};
const uint16_t virtani[]= 
{
0,0,0,0,0,0,0,0,0,27482,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,27482,27482,27482,3163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,27482,120,120,25037,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,50497,160,160,58101,42040,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,17993,168,168,58093,33856,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27482,9801,152,152,17066,34377,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,51986,26682,59209,128,128,17049,9793,27482,53018,0,0,0,0,0,0,0,0,0,0,0,46612,31005,27482,42569,176,176,41914,50769,27482,39197,46612,0,0,0,0,0,0,0,0,0,27482,19290,27482,62364,9801,176,176,33466,34121,62364,27482,27482,27482,0,0,0,0,0,0,0,27482,27482,62364,62364,62364,1609,128,128,24961,9545,62364,62364,62364,27482,27482,0,0,0,0,0,3171,62364,27482,62364,62364,29605,34361,61051,61051,46245,1593,62364,62364,62364,27482,62364,27482,0,0,0,27482,62364,62364,27482,3584,3584,3584,25897,793,793,49944,34089,3584,3584,3584,27482,62364,62364,27482,0,27482,62364,62364,62364,27482,3584,3584,3584,34064,9785,9785,51001,33800,61184,3584,3584,27482,62364,62364,62364,27482,2816,2816,2816,2816,2816,62364,62364,62364,1577,19290,19290,36203,50465,62364,62364,62364,2816,2816,2816,2816,2816,0,0,0,0,0,62364,62364,62364,26161,36996,36996,28804,50473,62364,62364,62364,0,0,0,0,0,0,0,0,0,0,50960,12544,61440,33544,10050,10050,59193,58112,45312,12288,26401,0,0,0,0,0,0,0,0,0,0,27755,28011,2114,29869,0,0,0,20355,11090,2642,44667,0,0,0,0,0,0,0,0,0,0,15982,65315,65315,24327,0,0,0,24327,65315,65315,24327,0,0,0,0,0,0,0,0,0,0,32327,32518,7703,24327,0,0,0,40527,24334,24367,24327,0,0,0,0,0,0,0,0,0,0,0,32518,7703,0,0,0,0,0,24334,24367,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t horz[]= 
{
0,0,0,0,0,0,0,2816,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,3171,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,27482,27482,27482,27482,0,0,0,0,0,0,0,0,0,24327,32327,15982,27755,50960,62364,62364,3584,3584,62364,62364,19290,46612,0,0,0,0,0,0,0,48503,32518,15873,53273,28011,12544,62364,62364,3584,3584,62364,62364,27482,31005,51986,0,0,0,0,0,0,24327,7703,57092,21010,2114,61440,62364,62364,3584,3584,29605,62364,62364,27482,26682,27482,27482,27482,27482,0,0,0,24327,24327,24327,29869,33544,26161,1577,34064,25897,34361,1609,9801,42569,59209,9801,17993,50497,27482,27482,0,0,0,0,0,0,10050,36996,19290,9785,793,61051,128,176,176,128,152,168,160,120,27482,27482,0,0,0,0,0,10050,36996,19290,9785,793,61051,128,176,176,128,152,168,160,120,27482,27482,0,0,0,0,0,59193,28804,36203,51001,49944,46245,24961,33466,41914,17049,17066,58093,58101,25037,27482,27482,0,24327,40527,24327,20355,58112,50473,50465,33800,34089,1593,9545,34121,50769,9793,34377,33856,42040,27482,3163,0,24327,24334,24066,53273,11090,45312,62364,62364,61184,3584,62364,62364,62364,27482,27482,27482,27482,27482,27482,0,0,24327,24367,16131,4113,2642,12288,62364,62364,3584,3584,62364,62364,27482,39197,53018,0,0,0,0,0,0,0,24327,24327,24327,44667,26401,62364,62364,3584,3584,62364,62364,27482,46612,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,27482,27482,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t horzani[]= 
{
0,0,0,0,0,0,0,2816,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,3171,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,27482,27482,27482,27482,0,0,0,0,0,0,0,0,0,0,32327,15982,27755,50960,62364,62364,3584,3584,62364,62364,19290,46612,0,0,0,0,0,0,0,0,32518,32518,65315,28011,12544,62364,62364,3584,3584,62364,62364,27482,31005,51986,0,0,0,0,0,0,0,7703,7703,65315,2114,61440,62364,62364,3584,3584,29605,62364,62364,27482,26682,27482,27482,27482,27482,0,0,0,0,24327,24327,29869,33544,26161,1577,34064,25897,34361,1609,9801,42569,59209,9801,17993,50497,27482,27482,0,0,0,0,0,0,10050,36996,19290,9785,793,61051,128,176,176,128,152,168,160,120,27482,27482,0,0,0,0,0,10050,36996,19290,9785,793,61051,128,176,176,128,152,168,160,120,27482,27482,0,0,0,0,0,59193,28804,36203,51001,49944,46245,24961,33466,41914,17049,17066,58093,58101,25037,27482,27482,0,0,40527,24327,20355,58112,50473,50465,33800,34089,1593,9545,34121,50769,9793,34377,33856,42040,27482,3163,0,0,24334,24334,65315,11090,45312,62364,62364,61184,3584,62364,62364,62364,27482,27482,27482,27482,27482,27482,0,0,0,24367,24367,65315,2642,12288,62364,62364,3584,3584,62364,62364,27482,39197,53018,0,0,0,0,0,0,0,0,24327,24327,44667,26401,62364,62364,3584,3584,62364,62364,27482,46612,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,27482,27482,27482,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,62364,27482,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2816,27482,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

// MAIN FUNCTION
int main()
{	
	//initialises game setup
	gameSetup();
	
	//main game function
	game();
}

void gameSetup()
{
	initClock();
	initSysTick();
	setupIO();
	initSound();
	initSerial();
}
	
void game()
{
	int shields = 3;

	while(1)
	{
	
	int projactive = 0;//Projectile toggle
	int objactive = 0;//meteor toggle, might make it able to create multiple using arrays just one for now

	//for ship
	int toggle = 0;
	int hmoved = 0;

	//For ship
	uint16_t x = 53;
	uint16_t y = 115;
	uint16_t oldx = x;
	uint16_t oldy = y;

	//For projectile
	uint16_t projx = 0;
	uint16_t projy = 0;
	uint16_t oldprojy = projy;// only moves up so no need for x update

	//For meteor
	uint16_t objx = 0;
	uint16_t objy = 0;
	uint16_t oldobjy = objy;// only moves down so no need for x update

	//score variable
	uint16_t score = 0;

	
	// calls the start game 
	startgame();
	
	char GameStartLog[] = {"Game has started"};

	fillRectangle(0,0,127,159,RGBToWord(0,0,0));
	putImage(x,y,21,21,virt,0,0); // Game starting point

	//srand(time(NULL));

	log(GameStartLog);

		while(1)
		{
			hmoved = 0;
			if ((GPIOB->IDR & (1 << 4))==0) // right pressed
			{					
				if (x < 102)
				{
					x = x + 2;
					hmoved = 1;
				}						
			}
			if ((GPIOB->IDR & (1 << 5))==0) // left pressed
			{			
				
				if (x > 3)
				{
					x = x - 2;
					hmoved = 1;
				}			
			}
			if ( (GPIOA->IDR & (1 << 8)) == 0 && projactive == 0) // up pressed and no projectile active
			{
				projactive = 1;//activates projectile
				projx = x + 10;
				projy = y - 5;
			}
			if ((hmoved))
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx,oldy,21,21,0);
				oldx = x;
				oldy = y;					
				if (hmoved)
				{
					if (toggle)
						putImage(x,y,21,21,virt,0,0);
					else
						putImage(x,y,21,21,virtani,0,0);
					
					toggle = toggle ^ 1;
				}

				// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
				
			}
			if(projactive==1)
			{
				//projectile movement 
				oldprojy = projy;
				projy = projy-2;
				fillRectangle(projx, oldprojy, 1,5,RGBToWord(0,0,0));
				fillRectangle(projx, projy, 1,5,RGBToWord(255,255,255));
				if (projy==0)
				{
				//checks to see if the projectile has reached the top of the screen
				fillRectangle(projx,projy,1,5,RGBToWord(0,0,0));
				projactive=0;//allows new projectile to be made
				}
			}	
			if(objactive==0)
			{
				objactive=1;
				objx=x+6;
				objy=5;
				objactive = 1;
				objx = rand() % 80+10;  // Generate a random x-coordinate for the meteor
				objy = 5;
			}
			if(objactive==1)
			{
				oldobjy=objy;
				objy++;
				fillRectangle(objx,oldobjy,5,5,RGBToWord(0,0,0));
				fillRectangle(objx,objy,5,5,RGBToWord(255,128,0));
				if(objy>140)
				{
				fillRectangle(objx,objy,5,5,RGBToWord(0,0,0));
				objactive=0;
				}
			}
			//detects if the meteor has hit the ship
			if (isInside(x,y,21,21,objx,objy) || isInside(x,y,21,21,objx,objy) || isInside(x,y,21,21,objx,objy) || isInside(x,y,21,21,objx,objy) )
				{
					//if you have lives left you lose them rather then losing the game
					if(shields>0)
					{
						shields--;
						objactive=0;
					}
					else
					{
						endgame(score);	
						break;	
					}
					
				}	

			//detects if the meteor has been hit by a missile
			if (isInside(objx,objy,5,5,projx,projy) || isInside(objx,objy,5,5,projx+5,projy) || isInside(objx,objy,5,5,projx,projy+5) || isInside(objx,objy,5,5,projx+5,projy+5) )
				{
					fillRectangle(objx,objy,30,30,RGBToWord(0,0,0));
					objactive=0;
					projactive=0;
					projx=0;
					projy=0;
					score = score + 1;
				}
			delay(15);
		}
	}
	
}
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}
// IO SETUP
void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
// function to start game
void startgame()
{
	fillRectangle(0, 0, 127, 159, RGBToWord(0, 0, 0));
    
	while(1)
		{
        // DISPLAY TITLE	
        printTextX2("Space", 21, 20, RGBToWord(255, 0, 0), 0);
		printTextX2("Shooter!", 21, 35, RGBToWord(255, 0, 255), 0);
		// PLAYER TITLE
		printText("Press Up", 27, 90, RGBToWord(0, 255, 0), 0);
		printText("to Begin!", 27, 100, RGBToWord(0, 0, 255), 0);
		// AUTHOR
		printText("by Harry & Josh", 17, 119, RGBToWord(0,255,0),0);

		// when up is pressed, start the game
		if  ((GPIOA->IDR & (1 << 8)) == 0)
		{	
			break;
		}
		}
		
}

void endgame(int x)
{
    fillRectangle(0, 0, 127, 159, RGBToWord(0, 0, 0));
    
   
	

		while(1)
		{
        
		// Print "Game Over!" message
        printTextX2("Game Over!", 7, 25, RGBToWord(255, 0, 0), 0);
		
        // Convert the integer x to a string
        char scoreText[20];  // Make sure the buffer is large enough to hold the text
        sprintf(scoreText, "Score: %d", x);

        // Print the score text
        printText(scoreText, 33, 50, RGBToWord(0, 255, 0), 0);

		printText("Try Again?", 27, 90, RGBToWord(0,255,0),0);
		printText("Press Down", 27, 100, RGBToWord(0,0,255),0);
		

		

			while  ((GPIOA->IDR & (1 << 11)) !=0 )
			{
		
			delay(40);
					
			
			}
		delay(100);
		break;
	    }
}
// Serial Logger
void log(char log[])
{
    // Serial Commiunication
    //log telling the user that the System has been intilised.
    int i = 0;
    while(log[i] != '\0')
    {
        eputchar(log[i]);
        i++;
    }
    eputs("\r\n");
}