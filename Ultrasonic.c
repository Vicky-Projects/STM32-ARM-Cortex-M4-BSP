#include <stm32f407xx.h>
#include "stdio.h"
#include "string.h"
float distance=0;
void tim2(void);
void GPIO(void);
void delayms(unsigned int milli);
void write(char ch);
void init(void);
void send_UART(char val); 
void print(char *string);
void send_UsART(char *msg);
void SysCLK(void);





void init(void){
	RCC->APB1ENR|=(1<<17);  //enable UE
	RCC->AHB1ENR|=(1<<0);  //ENABLE GPIOA
	GPIOA->AFR[0]=(7<<8)|(7<<12);  // AF7 PIN PA 2// AF7 PIN PA 3
	GPIOA->MODER|=(1<<5);  //ALTERNATE FUNCTION PIN2 TX
	GPIOA->MODER|=(1<<7);  // alternate func for pin3 RX
	USART2->BRR=0X0683;  //9600 BR AT 16 
	USART2->CR1=(1<<3)|(1<<2);  //TE RE ENA
	USART2->CR1|=(1<<13);  //UE ENA
	USART2->CR1|=(1<<5);
	USART2->CR1|=(1<<7);//TXEIE RXEIE
}


void send_UART(char val) {
	
			}
void send_UsART(char *msg) {
    for (int i = 0; i < (strlen(msg)); i++) {
        while((USART2->SR &(1<<7))==0); //TDREmpty
        write(msg[i]);
			
    }
}
void write(char ch){
	
	USART2->DR = ch;
	while((USART2->SR &(1<<7))==0); //TDREmpty
	
}

void print(char *string){
	while(*string)
		write(*string++);
}

void delayms(unsigned int milli){

	
	RCC->APB1ENR|=(1<<1);
	TIM3->PSC=15;
	TIM3->CR1|=(1<<0);
	TIM3->EGR|=(1<<0);
	while((TIM3->SR & (1<<0))==0);
	while(TIM3->CNT < milli);
}


void tim2(void){
	RCC->APB1ENR|=(1<<0);//tim2 clk en
	TIM2->PSC=15;
	TIM2->CR1&=(~(1<<4));//upcounter
	TIM2->ARR=0xFFFF-1;
}

void GPIO(void){
	RCC->AHB1ENR|=(1<<0);//gpioa clk en
	GPIOA->MODER|=(1<<0);//pa0 as output
	//GPIOA->OSPEEDR|=(3<<0)|(3<<2);//very high speed
	GPIOA->OTYPER=0;
	GPIOA->PUPDR=0;
	GPIOA->PUPDR|=(2<<2);//pa1 echo
}



int main(void){
	float pulse_start,pulse_end,pulse_dur;
	
		char messege1[50];
		char messege2[50];
		char messege3[50];
		char messege4[50];
	char messege5[50];
	//SysCLK();

	init();
	tim2();
	GPIO();
	//TIM2->EGR|=(1<<0);
	TIM2->CR1|=(1<<0);//en counter
	while(1){
		//TIM2->CNT=0;
	  sprintf(messege5,"CNT:%d",TIM2->CNT);
	  send_UsART(messege5);
	  write('\n');
		GPIOA->BSRR|=(1<<16);
		delayms(2);
		GPIOA->BSRR|=(1<<0);
		delayms(10);
		GPIOA->BSRR|=(1<<16);
		delayms(20);
		while((GPIOA->IDR & (1<<1))==0);
		pulse_start=TIM2->CNT;
		sprintf(messege1,"start:%.2f",pulse_start);
		send_UsART(messege1);
		write('\n');
		while((GPIOA->IDR & (1<<1))==1);
		pulse_end=TIM2->CNT;
		sprintf(messege2,"end:%.2f",pulse_end);
		send_UsART(messege2);
		write('\n');
		pulse_dur=(float)(pulse_end-pulse_start);
		sprintf(messege3,"dur:%.2f",pulse_dur);
		send_UsART(messege3);
		write('\n');
		distance=((pulse_dur)/58);
		sprintf(messege4,"distance:%.2f",distance);
		send_UsART(messege4);
		write('\n');
		delayms(65000);
		delayms(65000);
		delayms(65000);
		delayms(65000);
		delayms(65000);
    TIM2->EGR|=(1<<0);		
		distance=0;
	
}
	}
