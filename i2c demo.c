#include <stm32f407xx.h>


void sysclk(void);
void initI2C(void);
void start(void);
void write(unsigned int data);
void address(unsigned int add);
void stop(void);
void print(unsigned int *data,unsigned int size);
void delayms(int delay);



void sysclk(void){
	RCC->APB1ENR|=(1<<28); //power clock enable
	RCC->CR|=(1<<16);//HSE Clock enable
	while((RCC->CR &(1<<17))==0);//HSE FLAG
	RCC->CR|=(1<<24);//PLL ON
	RCC->PLLCFGR|=(1<<22);//PLL SOURCE
	RCC->PLLCFGR=0x00401004;//SETTING M & N VALUE 64MHZ
  while((RCC->CR & (1<<24))==0);//PLL FLAG
	RCC->CFGR|=(1<<1);//PLL AS SYSTEM CLK
  RCC->CFGR|=(9<<4);//AHB1 DIV VALUE
	while((RCC->CFGR & (1<<3))==0);// PLL AS SYS CLK FLAG
}


void initI2C(void){
	/*I2C CLK enable
Enable CLK for GPIO
Select Alternate Function
Select the Moder for GPIO
Set APB Frequency
Give CCR Value
Set Trise value
Enable I2C

*/
	
	RCC->APB1ENR|=(1<<21);
	RCC->AHB1ENR|=(1<<1);
	GPIOB->AFR[1]=(4<<0)|(4<<4);
	GPIOB->MODER=(2<<16)|(2<<18);
	I2C1->CR2=(16<<0);
	I2C1->CCR=(80<<0);
	I2C1->TRISE=(17<<0);
	I2C1->CR1=(1<<0);
}


void start(void){
	I2C1->CR1|=(1<<8);
	while((I2C1->SR1 & (1<<0))==0);
}


void address(unsigned int add){
	I2C1->DR=(add & 0xFF);
	while((I2C1->SR1 & (1<<1))==0);
	unsigned int temp=I2C1->SR1 | I2C1->SR2;
}

void write(unsigned int data){
	I2C1->DR =(data & 0xFF);
	while((I2C1->SR1 & (1<<7))==0);
}


void stop(void){
	I2C1->CR1|=(1<<9);
}

void print(unsigned int *data,unsigned int size){
	while((I2C1->SR1 & (1<<7))==0);
	while(size){
			while((I2C1->SR1 & (1<<7))==0);
		  I2C1->DR=(volatile unsigned int)*data++;
		  size--;
	}
	while((I2C1->SR1 & (1<<2))==0);
}

void delayms(int delay){
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}


int main(void){
  sysclk();
  initI2C();
while(1){
	for(int i=0;i<8;i++)
	{
start();
address(0x4E);
write(1<<i);
stop();
delayms(250);
}
}
	}	  
	
