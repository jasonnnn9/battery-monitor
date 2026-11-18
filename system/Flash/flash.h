#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"

#define DATA_EEPROM_START_ADDR  0x08080000  //EEPROM
 
#define STM32_FLASH_SIZE        0x80000          
#define STM32_FLASH_BASE        0x08000000       

  
#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE   1024                 
#else
#define STM32_SECTOR_SIZE   2048                 
#endif

 
#define STM32_FLASH_KEY1        0X45670123
#define STM32_FLASH_KEY2        0XCDEF89AB

 
static void stmflash_unlock(void);                   
static void stmflash_lock(void);                     
static uint8_t stmflash_get_error_status(void);      
static uint8_t stmflash_wait_done(uint32_t time);    
static uint8_t stmflash_erase_sector(uint32_t saddr);                    
static uint8_t stmflash_write_halfword(uint32_t faddr, uint16_t data);   

 
uint16_t Flash_ReadData_halfword(uint32_t faddr);                         
void Flash_ReadData(uint32_t raddr, uint16_t *pbuf, uint16_t length);     
void Flash_WriteData(uint32_t waddr, uint16_t *pbuf, uint16_t length);    

 
void test_write(uint32_t waddr, uint16_t wdata);

#endif


















