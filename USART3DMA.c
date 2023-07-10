#include "stm32f407xx.h"
#include "string.h"


void init(void);
void write(unsigned int ch);
void delayms(int delay);
void print(char *string);
unsigned int rece(void);
void SysCLK(void);
void DMA_INIT(void);
void DMA(uint32_t scradd, uint32_t desadd, uint16_t size);
void DMA1_Stream1_IRQHandler(void);


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
	
	//USART3->CR3|=(1<<7);//DMA TRANS EN
	USART3->CR3|=(1<<6);//DMA RECE EN
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


void DMA_INIT(void){
	RCC->AHB1ENR|=(1<<21);//DMA1 EN
	DMA1_Stream1->CR|=(1<<2)|(1<<3)|(1<<4);//EN INTRUPTS
	DMA1_Stream1->CR&=(~(3<<6));//P TO M
	//DMA1_Stream3->CR|=(1<<6);//M TO P
	DMA1_Stream1->CR|=(1<<8);//CIR MODE
	DMA1_Stream1->CR|=(1<<10);//MEM INCREMENT
	DMA1_Stream1->CR&=(~(3<<11));//PSIZE
	DMA1_Stream1->CR&=(~(3<<13));//MSIZE
	DMA1_Stream1->CR&=(~(3<<16));//PRIORITY LEVEL LOW
	DMA1_Stream1->CR|=(4<<25);//ch4 sel
}

void DMA(uint32_t scradd, uint32_t desadd, uint16_t size){
	DMA1_Stream1->NDTR=size;
	DMA1_Stream1->PAR=scradd;
	DMA1_Stream1->M0AR=desadd;
	DMA1_Stream1->CR|=(1<<0);//en stream
}

#define RXSIZE 20
static uint8_t RXbuf[20];
static uint8_t Mainbuf[50];
static uint8_t indx=0;

void DMA1_Stream1_IRQHandler(void)
	
	{
	if(DMA1->LISR & (1<<10))//IF HALF TRANSFER COMPLETE
	{
		memcpy (&Mainbuf[indx],&RXbuf[0],RXSIZE/2);
		DMA1->LIFCR|=(1<<10);//clear ht
		indx=indx+(RXSIZE/2);
		if(indx>49) indx=0;		
	}
	if(DMA1->LISR & (1<<11))//IF TRANSFER COMPLETE
	{
		memcpy (&Mainbuf[indx],&RXbuf[RXSIZE/2],RXSIZE/2);
		DMA1->LIFCR|=(1<<11);//clear ht
		indx=indx+(RXSIZE/2);
		if(indx>49) indx=0;		
	}
}
	
	
	
 


int main(void){
	
	//SysCLK();
	init();
	DMA_INIT();
	NVIC_SetPriority(DMA1_Stream1_IRQn,0);
	NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  DMA((uint32_t) &USART3->DR, (uint32_t) RXbuf, RXSIZE);	
	while(1){
		
			//write('h');
		
			
}
}





