#include <stm32f407xx.h>
#include "stdio.h"
#include "string.h"




void SYSCLK(void);
void ADC_init(void);
void ADC_EN(void);
void ADC_DIS(void);
void ADC_START(unsigned int CH);
void ADC_WAIT(void);
unsigned int ADC_GET(void);
void init(void);
void write(unsigned int ch);
void send_UsART(char *msg);





void SYSCLK(void){
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


void ADC_init(void){
	RCC->APB2ENR|=(1<<8);//EN ADC1 CLK
	RCC->AHB1ENR|=(1<<0);//EN GPIOA CLK
	ADC->CCR&=(~(1<<16))&(~(1<<17));// ADC CLK FR = 16/2=8MHZ
	ADC1->CR1|=(1<<8);//SCAN MODE AND 10BIT RES
	ADC1->CR1&=(~(1<<24))&(~(1<<25));//res 12bit
	ADC1->CR2|=(1<<1)|(1<<10);//EOC AND CC
	ADC1->CR2&=(~(1<<11));//RIGHT ALIGNMENT
	ADC1->SQR1|=(1<<20);//GOING TO USE 2 CHANNELS
	GPIOA->MODER|=(3<<2)|(3<<4);//ANALOG MODE FOR PIN PA 1,2
	ADC1->SMPR2|=(4<<3)|(4<<6);//sampling time ch 1,2
	
}

void ADC_EN(void){
	ADC1->CR2|=(1<<0);//ADC ON
}
void ADC_DIS(void){
	ADC1->CR2&=(~(1<<0));//ADC OFF
}


void ADC_START(unsigned int CH){
	ADC1->SQR3=0;
	ADC1->SQR3=(CH<<0);//SETTING THE SEQUENCE
	ADC1->SR=0;
	ADC1->CR2|=(1<<30);//start conversion
}

void ADC_WAIT(void){
	while((ADC1->SR & (1<<1))==0);
}

unsigned int ADC_GET(void){
	return ADC1->DR;
}
void init(void){
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->AFR[1]|=(7<<8)|(7<<12);//PIN PB 11 RX ,10 TX
	GPIOB->MODER|=(2<<20);
	GPIOB->MODER|=(2<<22);
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
			  write(msg[i]);
        while((USART3->SR &(1<<7))==0); //TDREmpty
        
			
    }
}
	


int main(void){
	char msg[50];
 

	unsigned int ADC_VAL[2]={0,0};
	
	
	init();
	ADC_init();
	ADC_EN();
	
	while(1){
		ADC_START(1);
		ADC_WAIT();
		ADC_VAL[0]=ADC_GET();
		//print func
		sprintf(msg,"ADC_CH1:%d",ADC_VAL[0]);
		send_UsART(msg);
		write('\n');
				
	
		
		
		ADC_START(2);
		ADC_WAIT();
		ADC_VAL[1]=ADC_GET();
		sprintf(msg,"ADC_CH2:%d",ADC_VAL[1]);
		send_UsART(msg);
		write('\n');
		
		
		for(int delay=0;delay<500000;delay++);
		
		
		
		
		
	}
}
		
	
	