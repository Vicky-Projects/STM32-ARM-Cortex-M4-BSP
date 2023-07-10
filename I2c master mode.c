#include <stm32f407xx.h>

void Sysclk(void);
void initI2C(void);
void i2cstart(void);
void i2cwrite(unsigned int data);
void i2cadd(unsigned int add);
void i2cstop(void);
void i2cprint(unsigned int *data, unsigned int size);
void delayms(int delay);

void Sysclk(void){
	
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
	
	RCC->APB1ENR|=(1<<21);//i2c clk enable
	RCC->AHB1ENR|=(1<<1);//ENABLE CLK FOR GPIO PORT B
	GPIOB->AFR[1]=(4<<0)|(4<<4);//I2C1 AS ALTERNATE FUNCTION
	GPIOB->MODER|=(2<<16);//PINS PB8,9 AS ALTERNATE FUNCTION
	GPIOB->MODER|=(2<<18);
	//GPIOB->OTYPER=(1<<8)|(1<<9);//OPEN DRAIN MODE PIN PB 8,9
	//GPIOB->OSPEEDR=(3<<16)|(3<<18);//HIGH SPEED MODE
	//GPIOB->PUPDR=(1<<16)|(1<<18);//PUSH PULL MODE
	
	
	I2C1->CR2=(16<<0);//SETTING APB1 FR
	I2C1->CCR=(80<<0);//SETING CLK CONTROL REGISTER
	I2C1->TRISE=(17<<0);//RISETIME
	I2C1->CR1|=(1<<0);//ENABLE I2C
	
	
}


void i2cstart(void){
	//I2C1->CR1|=(1<<10);
	I2C1->CR1|=(1<<8);//START I2C CONDITION
	while((I2C1->SR1 & (1<<0))==0);// START CONDITION FLAG
}




void i2cadd(unsigned int add){
	I2C1->DR=(add & 0xFF);//SEND ADDRESS TO DR
	while(!(I2C1->SR1 & (1<<1)));//WAIT FOR ADDRESS FLAG TO SET
	unsigned int temp = I2C1->SR1|I2C1->SR2;//READ SR1 & SR2 TO CLEAR
}

void i2cwrite(unsigned int data){
	
	I2C1->DR=(data & 0xFF);
	while((I2C1->SR1 & (1<<7))==0);//WAIT TO DATA REGI TO EMPTY
}

void i2cstop(void){
	
	I2C1->CR1|=(1<<9);//STOP GENERATION
}


void i2cprint(unsigned int *data, unsigned int size){
	while(!(I2C1->SR1 & (1<<7)));// WAIT FOR TXE TO SET
	while(size){
		while(!(I2C1->SR1 & (1<<7)));//WAIT FOR TXE BIT
		I2C1->DR=(volatile unsigned int)*data++;
		size--;
	}
	while(!(I2C1->SR1 & (1<<2)));//WAIT FOR BTF TO SET	
}
	


void delayms(int delay)
{
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}












int main(void){
	Sysclk();
	initI2C();
	while(1)
	{
		for(int i=0;i<8;i++)
		{
		
			i2cstart();
			i2cadd(0x4E);
	    i2cwrite(1<<i);
	    i2cstop();
			delayms(250);
	}
}
	
}	
	
	