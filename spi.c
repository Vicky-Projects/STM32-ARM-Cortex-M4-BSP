#include <stm32f407xx.h>


void SPI_INT(void);

void spi_init(void);
void gpio(void);
unsigned int trans(unsigned int ch);	
unsigned int rece(void);


void SPI_INT(void){
	NVIC->ISER[1]|=(1<<3);//EN INTERUPT SPI
}


	
	
void spi_init(void){
	RCC->APB2ENR|=(1<<12);//spi clk en
	RCC->AHB1ENR|=(1<<1);
	SPI_INT();
	SPI1->CR1=0;
	//BR = 16/2=8
	SPI1->CR1=(1<<0)|(1<<1);//CPOL=1,CPHA=1
	//DFF=0 8BIT DATA FRAME RESET STATE
	//MSB 1ST BY RESET STATE
	//SSM=0 RESET STATE
		GPIOB->MODER|=(2<<6)|(2<<8)|(2<<10)|(2<<14);// PINS 3,4,5,7 AF5
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20)|(5<<28);//AF5
	SPI1->CR2=(1<<7)|(1<<6)|(1<<4)|(1<<2);//EN TXEIE RXNEIE,SET FRF FOR SERIAL COM
	SPI1->CR1=(1<<2);//MASTER CONFI,SPI EN
	

}
void gpio(void){
	RCC->AHB1ENR|=(1<<1);
	GPIOB->MODER|=(2<<6)|(2<<8)|(2<<10)|(2<<14);// PINS 3,4,5,7 AF5
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20)|(5<<28);//AF5
}	

unsigned int trans(unsigned int ch){
	unsigned int rxdata=0;
	
	SPI1->CR1|=(1<<6);//SPI ENABLE
	
	SPI1->DR=(ch & 0xFF);
	
	while((SPI1->SR & (1<<1))==0);//wait for txe
	while((SPI1->SR & (1<<7))==1);//wait for BSY to reset
	rxdata=(SPI1->DR & 0xFF);
	SPI1->CR1&=~(1<<6);// SPI DISABLE
	return rxdata;
}

unsigned int rece(void){
	unsigned int mydata;
	while((SPI1->SR & (1<<0))==0);
	mydata= SPI1->DR;
  return mydata;
}	







int main(void){
	unsigned int rxd=0;
	spi_init();
	
	while(1){
		rxd=trans('H');
	}
}

	