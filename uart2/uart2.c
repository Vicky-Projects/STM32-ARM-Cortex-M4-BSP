#include <stm32f407xx.h> //uart5 PC12TX,PD2RX





void sysCLK(void);
void delayms(unsigned int milli);
void inituart5(void);
unsigned int sendchar(unsigned int mydata);





int main(){
	inituart5();
	UART5->CR1|=(1<<13);//UE ENABLE
	UART5->CR1|=(1<<7);//TXEIE TXE INTERRUPT ENABLE
	
	UART5->BRR=0X1117;// BR VALUE FOR 9600	
	//UART5->CR1|=(1<<13);//UE ENABLE
	UART5->CR1|=(1<<3);// TE ENABLE	
		//delayms(250);
	
	while(1){
		UART5->DR=0x41<<0;
	while((UART5->SR & (0x01 << 7))==0);
		//sendchar(0x41);
		//delayms(500);
		UART5->DR='H';
	while((UART5->SR & (0x01 << 7))==0);
		//sendchar('E');
		//delayms(500);
		
	}
}




void inituart5(void){
	sysCLK();
	RCC->AHB1ENR=(1<<2)|(1<<3);//ENABLE CLK FOR GPIO D.C
	RCC->APB1ENR|=(1<<20);//UART5 CLK ENABLE
	GPIOC->MODER|=(2<<24);//PC12TX AS ALTERNATE FUNCTION
	GPIOD->MODER|=(2<<4);//PD2RX AS ALTERNATE FUNCTION AF8
	GPIOD->AFR[0]=(8<<8);//PD2 AF8
	GPIOC->AFR[1]=(8<<16);//PC12 AF8
	RCC->APB1RSTR|=(1<<20);//RESET UART5
	RCC->APB1RSTR&=(~(1<<20));
	
	//
}
	
unsigned int sendchar(unsigned int mydata){
	
		
}




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

void sysCLK(void){
	RCC->APB1ENR|=(1<<28);// power interface clock enable
  PWR->CR=PWR_CR_VOS;// voltage regulato
  RCC->CR|=(1<<16);// hse on
	while((RCC->CR & (1<<17))==0);// check hse
	RCC->PLLCFGR|=0x24402A04;//seting divider values
	RCC->CR|=(1<<24);// pll on
	while((RCC->CR & (1<<25))==0);// check pll
	RCC->CFGR|=0xC0609402;//PLL as cpu clk,AHB,APB1,APB2
	while((RCC->CFGR &(1<<3))==0);//check pll is used as sysCLK 
}