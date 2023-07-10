#include <stm32f407xx.h>
void fr(void);
void fr_period(void);
void delayms(int delay);


void delayms(int delay)
{
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}
void fr(void){
	RCC->AHB1ENR|=(1<<0);//gpioa
	GPIOA->MODER|=(2<<2);//af for pa1 tim2 ch2 triger
	GPIOA->AFR[0]|=(1<<4);//af af1 tim2
	RCC->APB1ENR|=(1<<0);//tim2 EN
	TIM2->PSC=15;//1MHZ = 1us
	TIM2->ARR=10-1;
	TIM2->CCMR1&=(uint32_t)(~(3<<8));// seting o/p mode
	TIM2->CCMR1|=(3<<12);// Output compare 1 mode
	TIM2->CCR2=0;
	TIM2->CCER|=(1<<4);//en o/p
	TIM2->CCER|=(1<<5);//active low
	TIM2->CNT=0;
	TIM2->CR1|=(1<<0);//en counter
}

void fr_period(void){
	RCC->AHB1ENR|=(1<<0);//gpioa
	GPIOA->MODER|=(2<<4);//af for pa2 tim9 ch1 echo
	GPIOA->AFR[0]|=(3<<8);//af af3 tim9
	RCC->APB2ENR|=(1<<16);//tim9
	TIM9->PSC=15;
	TIM9->CCMR1|=(1<<0);//i/p capture mode
	TIM9->CCMR1|=(1<<6);//N=6
	TIM9->CCER|=(1<<0)|(3<<1)|(1<<3);//capture en input
	TIM9->CCR1=0;
	TIM9->CNT=0;
	TIM9->CR1|=(1<<0);//en counter
	
}
static float frequency=0;
static unsigned int start=0;
static unsigned int end=0;
static unsigned int duration=0;
static unsigned int distance=0;
static unsigned int distance2=0;
static unsigned int distance3=0;
int main(void){
	fr();
	
	fr_period();
	delayms(60);
	while(1){
		while((TIM9->SR & (1<<1))==0);//wait for edge to capture
		start=TIM9->CCR1;
		
		while((TIM9->SR & (1<<1))==0);//wait for edge to capture
		end=TIM9->CCR1;
		
		duration=(end-start);
		
		frequency=16.0f/(float)duration;
		distance=((duration*340)/2);
		distance2=(duration/58);
		distance3=(duration/148);
		
	}
}
		
		
		
		
		
		
	
	