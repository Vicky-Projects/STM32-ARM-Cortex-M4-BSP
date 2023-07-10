#include <stm32f407xx.h>


void Sysclk(void);
void GPIO(void);
void delayms(unsigned int milli);




void Sysclk(void){

	
	RCC->APB1ENR|=(1<<28);
	RCC->CR|=(1<<16);
	while((RCC->CR & (1<<17))==0);
	RCC->CR|=(1<<24);
	RCC->PLLCFGR|=(1<<22);
	RCC->PLLCFGR=0x00401004;
	while((RCC->CR & (1<<24))==0);
	RCC->CFGR|=(1<<1);
	RCC->CFGR|=(9<<4);
	while((RCC->CFGR & (1<<3))==0);
}
	
void GPIO(void){
	RCC->AHB1ENR|=(1<<3);
	GPIOD->MODER=(1<<24)|(1<<26)|(1<<28)|(1<<30);
}


void delayms(unsigned int milli){

	
	RCC->APB1ENR|=(1<<0);
	TIM2->PSC=15999;
	TIM2->CR1|=(1<<0);
	TIM2->EGR|=(1<<0);
	while((TIM2->SR & (1<<0))==0);
	while(TIM2->CNT < milli);
}



int main(void){
	Sysclk();
	GPIO();
	while(1){
		GPIOD->BSRR=(1<<12)|(1<<13)|(1<<14)|(1<<15);
		for(int i=0;i<5000000;i++);
		GPIOD->BSRR=(1<<28)|(1<<29)|(1<<30)|(1<<31);
		delayms(250);
	}
}
		
	
