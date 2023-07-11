#include <stm32f407xx.h>
void delayms(unsigned int milli);

void delayms(unsigned int milli){
	RCC->APB1ENR|=(1<<0);//enable clock for tim2
	TIM2->PSC=83999;//PR value
	TIM2->ARR|=0xFFFF-1;// auto reload value
	TIM2->CR1|=(1<<0);//enable the counter
	TIM2->CR1&=(~(1<<2));//URS=0 only counter overflow will give intrupt
	TIM2->CR1&=(~(1<<1));//UDIS=0;
	TIM2->EGR|=(1<<0);// re-initializing counter
	while((TIM2->SR & (1<<0))==0);// gives flag 1 when registers are updated
	TIM2->CNT=0;
	while(TIM2->CNT < milli);
	//TIM2->CR1=0x00;// disable the counter
}
	