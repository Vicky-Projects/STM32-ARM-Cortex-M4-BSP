#include <stm32f407xx.h>

void SysCLK(void);
void spi_init(void);
void GPIO(void);
void SPI_EN(void);
void SPI_TRANS(unsigned int data,int size);
void SPI_DIS(void);
void SPI_RECE(unsigned int *data,int size);
unsigned int rece(void);
void delayms(int delay);
void init(void);
void write(unsigned int ch);
void print(char *string);

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

void write(unsigned int ch){
	
	USART2->DR = (ch & 0xFF);
	while((USART2->SR &(1<<7))==0); //TDREmpty
	
}
void print(char *string){
	while(*string)
		write(*string++);
}



void delayms(int delay)
{
	int i;
	for(  ;delay>>0;delay--)
	for(i=0;i<3195;i++);
}


void spi_init(void){
	RCC->APB2ENR|=(1<<12);//spi1 clk en
	SPI1->CR1|=(1<<0)|(1<<1);//CPOL=1,CPHA=1
	SPI1->CR1&=(~(1<<2));// AS slave
	
	//SPI1->CR1|=(1<<3);//pclk 16mhz/4 = spiclk=4mhzs
	SPI1->CR1&=(~(1<<7));//MSB TRANSMITTED AT 1ST
	SPI1->CR1|=(1<<8)|(1<<9);//SSM,SSI = 1
	SPI1->CR1&=~(1<<10);//full duplex
	SPI1->CR1&=~(1<<11);//DFF 8 bit format
  SPI1->CR2=0;
	RCC->AHB1ENR|=(1<<1); //ENABLE CLK FOR GPIOB
	GPIOB->MODER=(2<<6)|(2<<8)|(2<<10);//PB 3,4,5 AS AF,9 AS I/P
	GPIOB->MODER&=(~(1<<18))&(~(1<<19));
	GPIOB->OSPEEDR=(2<<6)|(2<<8)|(2<<10)|(2<<12);//HIGH SPEED
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20);//AF5=0101 FOR PINS PB 3,
}


void GPIO(void){
	RCC->AHB1ENR|=(1<<1); //ENABLE CLK FOR GPIOB
	GPIOB->MODER=(2<<6)|(2<<8)|(2<<10);//PB 3,4,5 AS AF,9 AS I/P
	GPIOB->MODER&=(~(1<<18))&(~(1<<19));
	GPIOB->OSPEEDR=(2<<6)|(2<<8)|(2<<10)|(2<<12);//HIGH SPEED
	GPIOB->AFR[0]=(5<<12)|(5<<16)|(5<<20);//AF5=0101 FOR PINS PB 3,4,5
}

void SPI_EN(void){
	SPI1->CR1|=(1<<6);//SPI ENABLE
}

void SPI_DIS(void){
	SPI1->CR1&=~(1<<6);// SPI DISABLE
}


void SPI_RECE(unsigned int *data,int size){
	while(size){
		while((SPI1->SR & (1<<7))==1);//wait for BSY to reset
		SPI1->DR=0;//send dummy data
		while((SPI1->SR & (1<<0))==0);//wait till RXNE to set
		*data++=SPI1->DR;
		size--;
	}
}


unsigned int rece(void){
	unsigned int mydata=0;
	SPI1->DR=0;
	while((SPI1->SR & (1<<7))==1);//wait for BSY to reset
	while((SPI1->SR & (1<<0))==0);//wait till RXNE to set
  mydata=SPI1->DR;
	return mydata;
}

int main(void){
	unsigned int data;
	//SysCLK();
	//GPIO();
	init();
	spi_init();
	
	while(1){
		print("Hlo\n");
		if((GPIOB->IDR & (1<<9))==0){
		SPI_EN();
		data=rece();
			//delayms(50);
			write(data);
		SPI_DIS();		
		}
}
}
			