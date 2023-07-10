#include <stm32f407xx.h>


static volatile uint32_t myTicks=0;
void SysTick_init(void);
void Dms(uint32_t MS);
void SysTick_Handler(void);
void GPIO(void);






void GPIO(void){
	RCC->AHB1ENR|=(1<<3);
	GPIOD->MODER=(1<<24)|(1<<26)|(1<<28)|(1<<30);
}

void SysTick_init(void){
	SysTick->CTRL=0;
	SysTick->LOAD=(uint32_t)(16000-1);
	NVIC_SetPriority(SysTick_IRQn,1);
	SysTick->VAL=0;
	SysTick->CTRL|=(7<<0);
}
	

void Dms(uint32_t MS){
	myTicks=0;
	while(myTicks<=MS);
}

void SysTick_Handler(void){
	myTicks++;
}
	

int main(void){
	SysTick_init();
	GPIO();
	while(1){
		GPIOD->BSRR=(1<<12)|(1<<13)|(1<<14)|(1<<15);
		Dms(1000);
		GPIOD->BSRR=(1<<28)|(1<<29)|(1<<30)|(1<<31);
		Dms(3000);	
	}
}
		
	
	
	