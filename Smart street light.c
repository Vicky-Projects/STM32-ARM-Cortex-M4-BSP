#include <stm32f407xx.h>


void delayms(int delay){
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}
int main(void){
	RCC->AHB1ENR|=(1<<0);//clk gpioA
	GPIOA->MODER=0;//pa7 ldr,6 ir as input
	GPIOA->MODER|=(1<<2);//pa1 as o/p
	
	while(1){
		if((GPIOA->IDR & (1<<6))==0){
			if((GPIOA->IDR & (1<<7))==0){
				GPIOA->BSRR|=(1<<1);
			}
			}else{
				GPIOA->BSRR|=(1<<17);
			}
		
		}
		//delayms(50);
		
		
			
	
}
	
	