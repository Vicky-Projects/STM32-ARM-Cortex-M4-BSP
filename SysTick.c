#include <stm32f407xx.h>


void delayus(uint32_t US);

void dUs(uint32_t uS);
void dMs(unsigned int mS);
void SysTick_init(void);
void SysTick_Handler(void);
void GPIO(void);
static volatile uint32_t myTicks=0;


void SysTick_init(void){
	
	SysTick->CTRL=0;
  SysTick->LOAD=(uint32_t)(16000-1);
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  SysTick->VAL=0;
  SysTick->CTRL=(7<<0);	
}




void dUs(uint32_t uS){
	//SysTick->CTRL|=(1<<0);//en counter
	
	myTicks=0;
	while(myTicks<=(uS));
	//return (0UL);
	//SysTick->CTRL&=(~(1<<0));//dis counter
}


void dMs(unsigned int mS){

	//

	myTicks=0;
	while(myTicks<=(mS));
	//SysTick->CTRL&=(~(1<<0));//dis counter
	
}
void SysTick_Handler(void){
	
	myTicks++;
	//SysTick->CTRL&=(~(1<<16));
	//SysTick->CTRL=0;
	//SysTick->CTRL&=(~(1<<0));//dis counter
	
}

void GPIO(void){
	RCC->AHB1ENR|=(1<<3);
	GPIOD->MODER=(1<<24)|(1<<26)|(1<<28)|(1<<30);
}



void delayus(uint32_t US){
	SysTick->LOAD=(16-1);
	SysTick->VAL=0;
	SysTick->CTRL|=(1<<0);//en counter
	SysTick->CTRL|=(1<<2);//clk source,tickint
	for(int i=0;i<=US;i++){
		while(SysTick->CTRL & (1<<16)==0){}
		}
	SysTick->CTRL=0;
	}

	
int main(void){
	//SysTick_init();
	GPIO();
	while(1){
		GPIOD->BSRR=(1<<12)|(1<<13)|(1<<14)|(1<<15);
		//dMs(1000);
		delayus(1000000);
		GPIOD->BSRR=(1<<28)|(1<<29)|(1<<30)|(1<<31);
		//dMs(3000);
		delayus(3000000);
	}
}
		


	