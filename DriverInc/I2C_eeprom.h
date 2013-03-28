#ifndef _I2C_eeprom_H_BAB
#define _I2C_eeprom_H_BAB

#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Use the defines below the choose the EEPROM type */
//#define EE_M24C08  /* Support the device: M24C08. */
/* note: Could support: M24C01, M24C02, M24C04 and M24C16 if the blocks and 
   HW address are correctly defined*/
#define EE_M24C64_32  /* Support the devices: M24C32 and M24C64 */
//#define EE_24LC02	/* STM32F103ZE-EK开发板缺省配置的芯片是24LC02 */

/* Defines for the GPIO pins used for the I2C communication */
#define I2C_EE             I2C1
#define I2C_EE_CLK         RCC_APB1Periph_I2C1
#define I2C_EE_GPIO        GPIOB
#define I2C_EE_GPIO_CLK    RCC_APB2Periph_GPIOB
#define I2C_EE_SCL         GPIO_Pin_6
#define I2C_EE_SDA         GPIO_Pin_7

#ifdef EE_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Harware connection on the board (on STM3210C-EVAL board E0 = E1 = E2 = 0) */
 #define EEPROM_HW_ADDRESS     0xA0   /* E0 = E1 = E2 = 0 */ 
 #define I2C_Speed             200000
 #define I2C_SLAVE_ADDRESS7    0xA0
 #define I2C_FLASH_PAGESIZE    32

#elif defined (EE_M24C08)
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 
   EEPROM Addresses defines */
 #define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */ 
 #define I2C_Speed             200000
 #define I2C_SLAVE_ADDRESS7    0xA0
 #define I2C_FLASH_PAGESIZE    16

#elif defined (EE_24LC02)
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 
   EEPROM Addresses defines */
 #define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */ 
 #define I2C_Speed             200000
 #define I2C_SLAVE_ADDRESS7    0xA0
 #define I2C_FLASH_PAGESIZE    8
#endif /* EE_M24C64_32 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* void I2C_EE_Init(void);
void I2C_EE_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr);
void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void I2C_EE_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void); */

void eeprom_init(void);
void eeprom_write(u8 *pInBuf,u16 Addr,u16 NumOfBytes);
void eeprom_read(u8 *pOutBuf,u16 Addr,u16 NumOfBytes);
void eeprom_WaitStandbyState(void);
#endif
