#include <stm32f407xx.h>

#include "SysCLK.h"
#include "TIMER.h"




int main(){
	sysCLK();
	RCC->AHB1ENR|=(1<<0);//enable port A
	GPIOA->MODER|=(1<<6);//configure pin 3 as output
	GPIOA->OSPEEDR|=0x00;// set low speed mode
	while(1){
		GPIOA->BSRR|=(1<<3);
		delayms(75);
		GPIOA->BSRR|=(1<<19);//OR (1<<3)<<19;
		delayms(75);
	}	
}