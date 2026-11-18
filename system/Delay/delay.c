#include "delay.h"
#include "sys.h"


static u32 fac_us=0;							 
void delay_init(u8 SYSCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); 
	fac_us=SYSCLK;							
}

 
 
 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;          
    ticks = nus * 72;                				  
    told = SysTick->VAL;                      
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;         
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                       
            }
        }
    }


}

 
 
void delay_ms(uint16_t nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}
	 



































