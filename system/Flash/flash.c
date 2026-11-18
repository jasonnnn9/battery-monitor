#include "main.h"
#include "flash.h"
 




uint16_t Flash_ReadData_halfword(uint32_t faddr)
{
    return *(volatile uint16_t *)faddr;
}

 






void Flash_ReadData(uint32_t raddr, uint16_t *pbuf, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = Flash_ReadData_halfword(raddr);     
        raddr += 2;  
    }
}

 







void stmflash_write_nocheck(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
    uint16_t i;
    for (i = 0; i < length; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, waddr, pbuf[i]);
        waddr += 2;      
    }
}

 










uint16_t g_flashbuf[STM32_SECTOR_SIZE / 2];  
void Flash_WriteData(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
    uint32_t secpos;         
    uint16_t secoff;         
    uint16_t secremain;      
    uint16_t i;
    uint32_t offaddr;        
    FLASH_EraseInitTypeDef flash_eraseop;
    uint32_t erase_addr;     

    if (waddr < STM32_FLASH_BASE || (waddr >= (STM32_FLASH_BASE + STM32_FLASH_SIZE)))
    {
        return;      
    }

    HAL_FLASH_Unlock();      

    offaddr = waddr - STM32_FLASH_BASE;              
    secpos = offaddr / STM32_SECTOR_SIZE;            
    secoff = (offaddr % STM32_SECTOR_SIZE) / 2;      
    secremain = STM32_SECTOR_SIZE / 2 - secoff;      
    if (length <= secremain)
    {
        secremain = length;  
    }

    while (1)
    {
        Flash_ReadData(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE, g_flashbuf, STM32_SECTOR_SIZE / 2);  
        for (i = 0; i < secremain; i++)                                                               
        {
            if (g_flashbuf[secoff + i] != 0XFFFF)
            {
                break;       
            }
        }
        if (i < secremain)   
        { 
            flash_eraseop.TypeErase = FLASH_TYPEERASE_PAGES;      
            flash_eraseop.NbPages = 1;
            flash_eraseop.PageAddress = secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE;   
            HAL_FLASHEx_Erase( &flash_eraseop, &erase_addr);

            for (i = 0; i < secremain; i++)          
            {
                g_flashbuf[i + secoff] = pbuf[i];
            }
            stmflash_write_nocheck(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE, g_flashbuf, STM32_SECTOR_SIZE / 2);  
        }
        else
        {
            stmflash_write_nocheck(waddr, pbuf, secremain);  
        }
        if (length == secremain)
        {
            break;  
        }
        else        
        {
            secpos++;                
            secoff = 0;              
            pbuf += secremain;       
            waddr += secremain * 2;  
            length -= secremain;     
            
            if (length > (STM32_SECTOR_SIZE / 2))
            {
                secremain = STM32_SECTOR_SIZE / 2;  
            }
            else
            {
                secremain = length;  
            }
        }
    }

    HAL_FLASH_Lock();  
}

/******************************************************************************************/
 

 





void test_write(uint32_t waddr, uint16_t wdata)
{
    Flash_WriteData(waddr, &wdata, 1);  
}
