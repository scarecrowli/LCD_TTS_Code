#include "I2C_eeprom.h"

uint16_t EEPROM_ADDRESS;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the used I/O ports pin
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  /* Configure I2C_EE pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  I2C_EE_SCL | I2C_EE_SDA; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //复用开漏输出 
  GPIO_Init(I2C_EE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //复用开漏输出 
  GPIO_Init(I2C_EE_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(I2C_EE_GPIO,GPIO_Pin_5);//写保护失能
	
	
}

/**
  * @brief  I2C Configuration
  * @param  None
  * @retval None
  */
static void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //设置 I2C 的占空比  //I2C 快速模式 I2C_DutyCycle_2=Tlow / Thigh = 2
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;	//设置第一个设备自身地址，它可以是一个 7 位地址或者一个 10 位地址
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//使能应答（ACK）
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	   //定义了应答 7 位地址还是 10 位地址
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;			//设置时钟频率，这个值不能高于 400KHz。
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C_EE, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C_EE, &I2C_InitStructure);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
static void I2C_EE_Init()
{
  /* I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   
  
  /* GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
  
  /* GPIO configuration */
  GPIO_Configuration();

  /* I2C configuration */
  I2C_Configuration();

#if defined (EE_M24C64_32)
  /* Select the EEPROM address according to the state of E0, E1, E2 pins */
  EEPROM_ADDRESS = EEPROM_HW_ADDRESS;  
#elif defined (EE_M24C08)
  /* depending on the EEPROM Address selected in the i2c_ee.h file */
 #ifdef EEPROM_Block0_ADDRESS
  /* Select the EEPROM Block0 to write on */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
 #endif
  
 #ifdef EEPROM_Block1_ADDRESS
  /* Select the EEPROM Block1 to write on */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
 #endif

 #ifdef EEPROM_Block2_ADDRESS
  /* Select the EEPROM Block2 to write on */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
 #endif
  
 #ifdef EEPROM_Block3_ADDRESS
  /* Select the EEPROM Block3 to write on */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
 #endif 

  EEPROM_ADDRESS = EEPROM_HW_ADDRESS;  
#elif defined (EE_24LC02)
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;	
#endif /* EE_M24C64_32 */  
  
}

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
zhgmdh2009
  * @retval None
  */
static void I2C_EE_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

#if defined(EE_M24C64_32)  
  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));

#else /* 小容量EEPROM,只有1字节地址 */
  /* Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C_EE, WriteAddr);  
#endif
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C_EE, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read 
  *   from the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
static void I2C_EE_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C_EE, ENABLE);

#if defined (EE_M24C64_32)

  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((ReadAddr & 0xFF00) >> 8));    
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(ReadAddr & 0x00FF));    

#else
  /* Send the EEPROM's internal address to read from: Only one byte address */
  I2C_SendData(I2C_EE, ReadAddr);    
#endif /* EE_M24C08 */
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_EE, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C_EE, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C_EE);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_EE, ENABLE);
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
static void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

#if defined(EE_M24C64_32)
  
  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));
  
#else
  /* Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C_EE, WriteAddr);
#endif /* EE_M24C08 */  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C_EE, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
static void I2C_EE_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Read I2C_EE SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(I2C_EE, I2C_Register_SR1);

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2C_EE, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C_EE, I2C_FLAG_AF);
  
  /* STOP condition */    
  I2C_GenerateSTOP(I2C_EE, ENABLE);  
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
static void I2C_EE_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % I2C_FLASH_PAGESIZE;
  count = I2C_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
  NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
 
  /* If WriteAddr is I2C_FLASH_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE); 
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_FLASH_PAGESIZE aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Write the data conained in same page */
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();      
        
        /* Write the remaining data in the following page */
        I2C_EE_PageWrite((uint8_t*)(pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));
        I2C_EE_WaitEepromStandbyState();        
      }      
      else      
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();        
      }     
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
      NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}






/*******************************************************************/
/**API********************************/

/**
eeprom 初始化
*/
void eeprom_init(void)
{
	I2C_EE_Init();
}

/**
往eeprom中写数据
@param pInBuf 要写入数据的地址
@param Addr eeprom的地址
@param NumOfBytes 要写入多少个字节
*/
void eeprom_write(u8 *pInBuf,u16 Addr,u16 NumOfBytes)
{
	I2C_EE_BufferWrite(pInBuf, Addr, NumOfBytes); 
}

/**
从eeprom中读出数据
@param pOutBuf 要读出数据保存的地址
@param Addr eeprom的地址
@param NumOfBytes 要读出多少个字节
*/
void eeprom_read(u8 *pOutBuf,u16 Addr,u16 NumOfBytes)
{
	 I2C_EE_BufferRead(pOutBuf, Addr, NumOfBytes); 
}
/**
等待eeprom回到空闲状态
*/
void eeprom_WaitStandbyState(void)
{
	I2C_EE_WaitEepromStandbyState();
}
