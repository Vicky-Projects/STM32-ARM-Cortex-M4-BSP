#include <stm32f407xx.h>
#include "math.h"
static int i;
static uint32_t sine_val[100]={0};
#define PI 3.1415926
void tim2(void);
void GPIO(void);
void dac(void);
void dac_start(void);
static void setvalue(uint32_t data);
void delayms(int delay);





	
void GPIO(void){
	RCC->AHB1ENR|=(1<<0);//en gpioa
}
void delayms(int delay)
{
	int j;
	for(  ;delay>>0;delay--)
	for(j=0;j<3195;j++);
}
void dac_start(void){
	DAC1->CR|=(1<<0);//en dac
}
static void setvalue(uint32_t data){
	DAC1->DHR12R1=(data<<0);
}

void dac(void){
	RCC->APB1ENR|=(1<<29);//en clk dac
	DAC1->CR=0;
	DAC1->CR|=(1<<2);//en triger
	DAC1->CR|=(4<<3);//tim2 as triger
	
}

void tim2(void){
	RCC->APB1ENR|=(1<<0);//en clk tim2
	TIM2->PSC=16-1;
	TIM2->CR1=0;
	TIM2->ARR=100-1;
	TIM2->SMCR|=(0x1<<12);//external trigger prescalar div2
	TIM2->CR2|=(2<<4);//en update event
	TIM2->SMCR&=(~(1<<7));//disable master/slave mode disable
	TIM2->CR1|=(1<<0);//en counter
}

	
	
	
	
	
	
	
	
	
int main(void){
	GPIO();	
	dac();	
	tim2();
	dac_start();	
	while(1){
		for(i=0;i<100;i++){
		sine_val[i]=((sin(i*2*PI/100)+1)*(4095/2));
			setvalue(sine_val[i]);
	}
		
		
	}
}



	
	
	
	
	
	
	
	
