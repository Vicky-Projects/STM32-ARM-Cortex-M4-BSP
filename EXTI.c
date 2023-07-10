#include <stm32f407xx.h>
void EXTI3_IRQHandler(void);
void Interrupt(void);
void GPIO(void);
	




void GPIO(void){
	RCC->AHB1ENR|=(1<<3);// en clk gpiod
	GPIOD->MODER=0;
	GPIOD->PUPDR|=(1<<6);//pull-up pd3
}

void Interrupt(void){
	RCC->APB2ENR|=(1<<14);//en sys clk control
	SYSCFG->EXTICR[1]|=(3<<12);//exti line 3
	EXTI->IMR|=(1<<3);//disabled mask
	EXTI->RTSR|=(1<<3);//rising edge
	NVIC_SetPriority(EXTI3_IRQn,1);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

static int flag=0;
static int count=0;

void EXTI3_IRQHandler(void){
	if(EXTI->PR & (1<<3))
	{
		EXTI->PR|=(1<<3);// clear pending register
		flag=1;
	}
}
	

int main(void){
	GPIO();
	Interrupt();
	while(1){
		if(flag==1){
			count++;
			flag=0;
		}
	}
}
	
	