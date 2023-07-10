#include <stm32f407xx.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void ADC_WAIT(void);
void dma_config(uint32_t srcadd,uint32_t desadd, uint16_t size);
void dma_init(void);

void ADC_EN(void);
void ADC_START(void);
void adc_init(void);
void sysclk(void);
void ADC_DIS(void);
void delayms(int delay);
void write(unsigned int ch);
void send_UsART(char *msg);
void init_u(void);




void delayms(int delay){
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}

void sysclk(void){
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
void init_u(void){
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->AFR[1]|=(7<<8)|(7<<12);
	GPIOB->MODER|=(2<<20);
	GPIOB->MODER|=(2<<22);
	
	//USART3->CR1|=(1<<7);//DMA TRANS EN
	USART3->BRR=0x0683;
	USART3->CR1|=(1<<3);
	USART3->CR1|=(1<<2);
	USART3->CR1|=(1<<13);
	USART3->CR1|=(1<<5)|(1<<7);
}






void send_UsART(char *msg) {
    for (int i = 0; i < (strlen(msg)); i++) {
        while((USART3->SR &(1<<7))==0); //TDREmpty
        write(msg[i]);
			
    }
}
void write(unsigned int ch){
	
	USART3->DR = (ch  & 0xFF);
	while((USART3->SR &(1<<7))==0); //TDREmpty
	
}

void adc_init(void){
	RCC->APB2ENR|=(1<<8);//en adc1 clk
	RCC->AHB1ENR|=(1<<0);//en gpioa clk
	ADC->CCR|=(3<<16);// 64/8=8 mhz
	ADC1->CR1|=(1<<8);//scan mode
	ADC1->CR1&=(~(1<<24))&(~(1<<25));//res 12bit
	ADC1->CR2|=(1<<1)|(1<<10);//en cc,eoc
	ADC1->CR2&=(~(1<<11));//right align
	ADC1->SQR1|=(2<<20);// using ch 3
	GPIOA->MODER|=(3<<2);//analog mode
	GPIOA->MODER|=(3<<4);
	ADC1->SMPR2|=(4<<3)|(4<<6);//sampling time ch 1,2
	ADC1->SMPR1|=(4<<18);//ST 12US
	ADC1->SMPR1|=(4<<21);//st ch17
	ADC->CCR|=(1<<23);//TS EN
	ADC1->CR2|=(1<<8);//en mode dma, conti req
	ADC1->CR2|=(1<<9);
	ADC1->SQR3|=(1<<0);//seq for ch 1,2
	ADC1->SQR3|=(2<<5);
	ADC1->SQR3|=(16<<10); //CH 16 TS
	ADC1->SQR3|=(17<<15);//ch 17
	
}

void ADC_EN(void){
	ADC1->CR2|=(1<<0);//ADC ON
	//delayms(1000);
}



void dma_init(void){
	RCC->AHB1ENR|=(1<<22);//en clk dma
	DMA2_Stream0->CR&=(~(1<<0));//en dma
	DMA2_Stream0->CR&=(~(3<<6));
	DMA2_Stream0->CR|=(1<<8);//circular mode
	DMA2_Stream0->CR|=(1<<10);// MEMORY ADD INCREMENT
	DMA2_Stream0->CR|=(1<<11)|(1<<13);//data , mem size
	DMA2_Stream0->CR&=(~(7<<25));//sel ch0 FOR DMA
}

void ADC_START(void){
	
	ADC1->SR=0;
	ADC1->CR2|=(1<<30);//start conversion
}


void dma_config(uint32_t srcadd,uint32_t desadd, uint16_t size){
	DMA2_Stream0->NDTR=(size & 0xFFFF);
	DMA2_Stream0->PAR=(srcadd & 0xFFFFFFFF);
	DMA2_Stream0->M0AR=(desadd & 0xFFFFFFFF);
	DMA2_Stream0->CR|=(1<<0);//en dma
}
void ADC_WAIT(void){
	while((ADC1->SR & (1<<1))==0);
}


void ADC_DIS(void){
	ADC1->CR2&=(~(1<<0));//ADC OFF
}




  
static char msg[50];

int main(void){
	uint16_t rxdata[3];
  float Temperature;
	init_u();
	adc_init();
	ADC_EN();
	dma_init();
	dma_config((uint32_t) &ADC1->DR,(uint32_t) rxdata, 3);
	ADC_START();
	while(1){
	Temperature  = (((float)(3.3*rxdata[2]/(float)4095)-0.76)/0.0025)-10;
	
		
		sprintf(msg,"ch1:%d",rxdata[0]);
		send_UsART(msg);
		write('\n');
		
		
		sprintf(msg,"ch2:%d",rxdata[1]);
		send_UsART(msg);
		write('\n');
		
		
		sprintf(msg,"ch16:%d",rxdata[2]);
		send_UsART(msg);
		write('\n');
		
		
		sprintf(msg,"TEMP:%.2f",Temperature);
		send_UsART(msg);
		write('\n');
	
		delayms(2000);
		 
	}
	  
}

