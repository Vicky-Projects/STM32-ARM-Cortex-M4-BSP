#include <stm32f407xx.h>



static volatile unsigned int time=0;
int main(void){
	RCC->AHB1ENR|=(1<<0);//en clk gpioa
	GPIOA->MODER|=(2<<10);//pa5 as AF
	GPIOA->AFR[0]|=(1<<20);//AF1 pa5
	RCC->APB1ENR|=(1<<0);//tim2 clk en
	TIM2->PSC=15999;
	TIM2->CCMR1|=(1<<0);//channel 1 as i/p ti2
	TIM2->CCER|=(1<<0);//en capture
	TIM2->CR1|=(1<<0);
	while(1){
		while(!(TIM2->SR & (1<<1)))
			time=TIM2->CCR1;
		
	}
			
	
	
}