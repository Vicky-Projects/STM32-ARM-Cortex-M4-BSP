#include <stm32f407xx.h>

void sysCLK(void);


void sysCLK(){
	RCC->APB1ENR|=(1<<28);// power interface clock enable
  PWR->CR=PWR_CR_VOS;// voltage regulato
  RCC->CR|=(1<<16);// hse on
	while((RCC->CR & (1<<17))==0);// check hse
	RCC->PLLCFGR|=0x24402A04;//seting divider values
	RCC->CR|=(1<<24);// pll on
	while((RCC->CR & (1<<25))==0);// check pll
	RCC->CFGR|=0xC0609402;//PLL as cpu clk,AHB,APB1,APB2
	while((RCC->CFGR &(1<<3))==0);//check pll is used as sysCLK 
}
	
	
	