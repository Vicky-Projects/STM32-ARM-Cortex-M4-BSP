#include <stm32f407xx.h>

void delay(void);
void inituart(void);
void sendchar(int ch);
void delay(void){
	unsigned int i=0;
	for(i=0;i<0xFFF;i++);
}
	
void inituart(void){
	RCC->APB1ENR|=(1<<28);//power clock enable
	PWR->CR=PWR_CR_VOS;//voltage regulator
	RCC->CR|=(1<<16);//hse on
	while((RCC->CR & (1<<17))==0);//check hse
	RCC->CFGR|=(1<<0);//hse as system clk
	while((RCC->CFGR & (1<<2))==0);//check hse is used as sys clk
	RCC->AHB1ENR|=(1<<0);//gpio A clk enable 
	GPIOA->MODER=(2<<1)|(2<<3);// seting alternate func for pins pa0,pa1
	GPIOA->OSPEEDR=(1<<0)|(1<<1)|(1<<2)|(1<<3);//configure high speed
	GPIOA->AFR[0]=(8<<3)|(8<<7);// seting alternate func AF8
	RCC->APB1ENR=(1<<19);//uart4 clk enable
	UART4->BRR=0x45;//baudrate value for 115200
	UART4->CR1|=(1<<3);//enable TE
	UART4->CR1=(1<<13)|(1<<7);// enable uart,enable TXEIE	
}



void sendchar(int ch){
	while(!(UART4->SR & (1<<7)));//TXE data is clear
	UART4->DR=(ch & 0xFF);	
}





int main(){
	inituart();
	delay();
	while(1){
	sendchar('H');
	sendchar('E');
	sendchar('L');
	sendchar('L');
	sendchar('O');
	}
}
	
	
	
	
	
	