#include <stm32f407xx.h>
unsigned int delay;

void sysclk(void);
void GPIO(void);
void sysclk(){
	RCC->APB1ENR|=(1<<28);//power enable
	PWR->CR=PWR_CR_VOS;// voltage regulator
  RCC->CR|=(1<<16);//HSE ON
	while((RCC->CR & (1<<17))==0);//check crystal is ready
	RCC->CFGR|=0x80400001;//select oscillator as system clock
	while((RCC->CFGR & (1<<2))==0);//check oscillator is selected as system clock
	RCC->AHB1ENR|=(1<<0);//enable clk to port B
}

void GPIO(){
	GPIOA->MODER|=(1<<6);//configure pin 3 as gpio 
	//GPIOB->OTYPER|=(1<<3);
	//GPIOB->ODR|=(1<<3);
	//GPIOB->PUPDR|=(1<<7);//reset pin
	GPIOA->OSPEEDR|=0x00;//set medium speed
	
}

	
int main(void){
	sysclk();
	GPIO();
	while(1){
	GPIOA->BSRR|=(1<<3);	
	for(delay=0;delay<20000;delay++);
	GPIOA->BSRR=(1<<19);
	for(delay=0;delay<500000;delay++);
	}
}
	