#include "main.h"
#include "Spi.h"

SPI_HandleTypeDef hspi1,hspi2,hspi3;

 
/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
#if(Hard_SPI)		
void MX_SPIx_Init(void)
{
#if(SPI_1)	
	 MX_SPI1_Init();
#endif	
#if(SPI_2)
	 MX_SPI2_Init();
#endif	
#if(SPI_3)	
	 MX_SPI3_Init();
#endif	
}

void MX_SPI1_Init(void)
{
	  __HAL_RCC_SPI1_CLK_ENABLE();  

    hspi1.Instance = SPI1;                               		  /* SPI2 */
    hspi1.Init.Mode = SPI_MODE_MASTER;                         
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;               
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                   
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;                
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;                     
    hspi1.Init.NSS = SPI_NSS_SOFT;                             
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;  
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                    
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                    
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;    
    hspi1.Init.CRCPolynomial = 7;                              
    HAL_SPI_Init(&hspi1);                                      

    __HAL_SPI_ENABLE(&hspi1);  
//	  spi2_read_write_byte(0Xff); 
}	

void MX_SPI2_Init(void)
{
	  __HAL_RCC_SPI2_CLK_ENABLE();  

    hspi2.Instance = SPI2;                               		  /* SPI2 */
    hspi2.Init.Mode = SPI_MODE_MASTER;                         
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;               
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;                   
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;                
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;                     
    hspi2.Init.NSS = SPI_NSS_SOFT;                             
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;  
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;                    
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;                    
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;    
    hspi2.Init.CRCPolynomial = 7;                              
    HAL_SPI_Init(&hspi2);                                      

    __HAL_SPI_ENABLE(&hspi2);  
//	  spi2_read_write_byte(0Xff); 
}	

//void MX_SPI3_Init(void)
//{
 

//    hspi3.Instance = SPI3;                               		  /* SPI2 */
 
 
 
 
 
 
 
 
 
 
 
 

 
////	  spi2_read_write_byte(0Xff); 
//}	


 





void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{		
	GPIO_InitTypeDef GPIO_InitStruct = {0};

if(hspi->Instance==SPI1)
//  else if(hspi->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */
//		SPI1_CS_GPIO_CLK_ENABLE();
//		GPIO_InitStruct.Pin = SPI1_CS_GPIO_PIN;              
//		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
//		GPIO_InitStruct.Pull = GPIO_PULLUP;               
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
//		HAL_GPIO_Init(SPI1_CS_GPIO_PORT, &GPIO_InitStruct);  

//		SPI1_CS(Hi);

  /* USER CODE END SPI2_MspInit 1 */
	spi1_set_speed(SPI_SPEED_2);
  }
	
	
if(hspi->Instance==SPI2)
//  else if(hspi->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */
		SPI2_CS_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = SPI2_CS_GPIO_PIN;              
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
		GPIO_InitStruct.Pull = GPIO_PULLUP;               
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
		HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStruct);  

//		SPI2_CS(Hi);

		spi2_set_speed(SPI_SPEED_2);
  /* USER CODE END SPI2_MspInit 1 */
  }
	
//	if(hspi->Instance==SPI3)
////  else if(hspi->Instance==SPI2)
//  {
//  /* USER CODE BEGIN SPI2_MspInit 0 */

//  /* USER CODE END SPI2_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_SPI3_CLK_ENABLE();

//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**SPI2 GPIO Configuration
//    PB3     ------> SPI3_SCK
//    PB4     ------> SPI3_MISO
//    PB5     ------> SPI3_MOSI
//    */
//    GPIO_InitStruct.Pin  = GPIO_PIN_3|GPIO_PIN_5;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//		GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin  = GPIO_PIN_4;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /* USER CODE BEGIN SPI3_MspInit 1 */
//		SPI3_CS_GPIO_CLK_ENABLE();
//		GPIO_InitStruct.Pin = SPI3_CS_GPIO_PIN;              
//		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
//		GPIO_InitStruct.Pull = GPIO_PULLUP;               
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
//		HAL_GPIO_Init(SPI3_CS_GPIO_PORT, &GPIO_InitStruct);  

////		SPI3_CS(Hi);
//		spi3_set_speed(SPI_SPEED_2);	
//  /* USER CODE END SPI2_MspInit 1 */
//  }
			
}

 







void spi1_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));  
    __HAL_SPI_DISABLE(&hspi1);              
    hspi1.Instance->CR1 &= 0XFFC7;          
    hspi1.Instance->CR1 |= speed << 3;      
    __HAL_SPI_ENABLE(&hspi1);               
	
	
}

void spi2_set_speed(uint8_t speed)
{

    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));  
    __HAL_SPI_DISABLE(&hspi2);              
    hspi2.Instance->CR1 &= 0XFFC7;          
    hspi2.Instance->CR1 |= speed << 3;      
    __HAL_SPI_ENABLE(&hspi2);               			
	
}

void spi3_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));  
    __HAL_SPI_DISABLE(&hspi3);              
    hspi3.Instance->CR1 &= 0XFFC7;          
    hspi3.Instance->CR1 |= speed << 3;      
    __HAL_SPI_ENABLE(&hspi3);               		
	
}
 




#if(SPI_3)	
uint8_t spi3_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi3, &txdata, &rxdata, 1, 1000);
    return rxdata;  
}
#endif

#if(SPI_2)	
uint8_t spi2_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi2, &txdata, &rxdata, 1, 1000);
    return rxdata;  
}
#endif

#if(SPI_1)	
uint8_t spi1_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 1000);
    return rxdata;  
}
#endif

#endif

#if(Soft_SPI)	
void MX_SPIx_Init(void)
{		
  GPIO_InitTypeDef GPIO_InitStruct;
#if(Soft_SPI_1)	
	SPI2_SCK_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI2_SCK_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);   

	SPI2_MISO_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI2_MISO_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;      
	HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);    

	SPI2_MOSI_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI2_MOSI_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);    
	
	SPI2_CS_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI2_CS_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStruct);   
	SPI2_CS(Hi);
#endif	
	SPI3_SCK_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI3_SCK_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI3_SCK_GPIO_PORT, &GPIO_InitStruct);   

	SPI3_MISO_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI3_MISO_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;        
	HAL_GPIO_Init(SPI3_MISO_GPIO_PORT, &GPIO_InitStruct);    

	SPI3_MOSI_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI3_MOSI_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI3_MOSI_GPIO_PORT, &GPIO_InitStruct);    
	
	SPI3_CS_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = SPI3_CS_GPIO_PIN;              
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;       
	GPIO_InitStruct.Pull = GPIO_PULLUP;               
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SPI3_CS_GPIO_PORT, &GPIO_InitStruct);  


//	SPI3_CS(Hi);
	
}

#if(Soft_SPI_1)	
void simulate_spi2_write_byte(u8 data)
{
     u8 kk;
		 SPI2_SCK(Lo);
     SIMULATE_DELAY_US;    
     SIMULATE_DELAY_US;
	   SIMULATE_DELAY_US;
//		delay_us(3);
    for(kk=0;kk<8;kk++)
    {
    if((data&0x80)==0x80) SPI2_MOSI(Hi);
    else         SPI2_MOSI(Lo);
			
    SIMULATE_DELAY_US; 
//	  delay_us(1);			
    SPI2_SCK(Hi);
    SIMULATE_DELAY_US;
//		delay_us(1);	
    SPI2_SCK(Lo); 
    data = data<<1;
    }
 
}
 
uint8_t simulate_spi2_read_byte(void)
{
    uint8_t kk=0, ret=0;
    SPI2_SCK(Lo);
	  SIMULATE_DELAY_US;  
	  SIMULATE_DELAY_US;
	  SIMULATE_DELAY_US;
 
    for(kk=0;kk<8;kk++)
    {
	  ret = ret<<1; 
    SIMULATE_DELAY_US;
    SPI2_SCK(Hi);
		SIMULATE_DELAY_US;
    if(SPI2_MISO) ret |= 0x01;
    SPI2_SCK(Lo);
    }
 
    return ret;
}
#endif	

void simulate_spi3_write_byte(uint8_t data)
{
     u8 kk;
		 SPI3_SCK(Lo);
     SIMULATE_DELAY_US;    
     SIMULATE_DELAY_US;
	   SIMULATE_DELAY_US;
//		delay_us(3);
    for(kk=0;kk<8;kk++)
    {
    if((data&0x80)==0x80) 
		{
			SPI3_MOSI(Hi);
		}
    else
		{
			SPI3_MOSI(Lo);
		}			
			
    SIMULATE_DELAY_US; 
//	  delay_us(1);			
    SPI3_SCK(Hi);
    SIMULATE_DELAY_US;
//		delay_us(1);	
    SPI3_SCK(Lo); 
    data = data<<1;
    }
 
}
 
u8 simulate_spi3_read_byte(void)
{
    u8 kk=0, ret=0;
    SPI3_SCK(Lo);
	  SIMULATE_DELAY_US;  
	  SIMULATE_DELAY_US;
	  SIMULATE_DELAY_US;
 
    for(kk=0;kk<8;kk++)
    {
	  ret = ret<<1; 
    SIMULATE_DELAY_US;
    SPI3_SCK(Hi);
		SIMULATE_DELAY_US;
    if(SPI3_MISO) ret |= 0x01;
    SPI3_SCK(Lo);
    }
 
    return ret;
}

#endif
