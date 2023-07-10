#include "stm32f407xx.h"

void init(void);
void write(unsigned int ch);
void delayms(int delay);
void print(char *string);
unsigned int rece(void);
void SysCLK(void);

void SysCLK(void){
	RCC->APB1ENR|=(1<<28); //power clock enable
	RCC->CR|=(1<<16);//HSE Clock enable
	while((RCC->CR &(1<<17))==0);//HSE FLAG
	RCC->CR|=(1<<24);//PLL ON
	RCC->PLLCFGR|=(1<<22);//PLL SOURCE
	RCC->PLLCFGR=(4<<0)|(64<<6);//SETTING M & N VALUE 64MHZ
  while((RCC->CR & (1<<25))==0);//PLL FLAG
	RCC->CFGR|=(1<<1);//PLL AS SYSTEM CLK
  RCC->CFGR|=(9<<4);//AHB1 DIV VALUE
	while((RCC->CFGR & (1<<3))==0);// PLL AS SYS CLK FLAG
}

void init(void){
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->AFR[1]|=(7<<8)|(7<<12);//PIN PB 11 RX ,10 TX
	GPIOB->MODER|=(2<<20);
	GPIOB->MODER|=(2<<22);
	
	//USART3->CR1|=(1<<7);//DMA TRANS EN
	USART3->BRR=0x0683;
	USART3->CR1|=(1<<3);
	USART3->CR1|=(1<<2);
	USART3->CR1|=(1<<13);
	USART3->CR1|=(1<<5)|(1<<7);
}

void write(unsigned int ch){
	
	USART3->DR = (ch & 0xFF);
	while((USART3->SR &(1<<7))==0); //TDREmpty
	
}
void send_UsART(char *msg) {
    for (int i = 0; i < (strlen(msg)); i++) {
        while((USART2->SR &(1<<7))==0); //TDREmpty
        write(msg[i]);
			
    }
}

void delayms(int delay)
{
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}
	
void print(char *string){
	while(*string)
		write(*string++);
}

unsigned int rece(void)
{
	unsigned int mydata;
	while((USART2->SR & (1<<5))==0){} // wait for RXNE bit to set	
  mydata = USART2->DR;
	return mydata;
}



int main(void){
	SysCLK();
	init();	
	while(1){
		//unsigned int data=rece();
		write('h');			
}
}




