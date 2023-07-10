#include <stm32f407xx.h>

#define led 15
#define period 200
#define duty 5;

int i=0;

int main(void){
	while(1){
	for(i=0;i<200;i++){
	RCC->AHB1ENR|=(1<<3);//gpiod clk en
	RCC->APB1ENR|=(1<<2);//tim4 clk en
	GPIOD->MODER|=(2<<30);//pd15 as AF
	GPIOD->AFR[1]|=(2<<28);//AF2 tim 4
	TIM4->CCMR2=0;//CC CHANNEL 4 CONFI AS O/P MODE CCMR2=0
	TIM4->CCER&=(~(1<<13));//CCER ACTIVE HIGH OUTPUT POLARITY=INITIALE STATE
	TIM4->CCMR2|=(1<<13)|(1<<14);//PWM MODE 1
	TIM4->PSC=1599;//PR VALUE FOR 1KHZ
	TIM4->ARR = period; // ON TIME
	TIM4->CCR4 = i; //OFF TIME
	for(int delay=0;delay<500000;delay++);
	TIM4->CCMR2|=(1<<11);//EN PRELOAD
	TIM4->CR1|=(1<<7);//ENABLE AUTO-RELOAD PRELOAD 
	//SELECT THE COUNTING MODE COUNTS UP BY RESET VALUE
	TIM4->CCER|=(1<<12);//ENABLE OUTPUT CH4
	TIM4->CR1|=(1<<0);//COUNTER ENABLE
	
	}
}
}
	
	