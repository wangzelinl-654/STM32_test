#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
#include "stdio.h"

 
#define GPIO_CLOCK     		RCC_APB2Periph_GPIOA     
#define GPIO_WHAT       	GPIOA					 
#define GPIO_PIN        	GPIO_Pin_1				 
#define GPIO_PIN_DATA   	1						 
#define	DS18B20_DQ_OUT      PAout(1)                
#define	DS18B20_DQ_IN       PAin(1)             


#if (GPIO_PIN_DATA < 8 )
#define GPIO_DATA (0xFFFFFFFF & (~(0x0f << 4 * GPIO_PIN_DATA)))
#define DS18B20_IO_IN()  {GPIO_WHAT->CRL&=GPIO_DATA;GPIO_WHAT->CRL|=8<<4*GPIO_PIN_DATA;}
#define DS18B20_IO_OUT() {GPIO_WHAT->CRL&=GPIO_DATA;GPIO_WHAT->CRL|=3<<4*GPIO_PIN_DATA;}
#endif

#if (GPIO_PIN_DATA >= 8 )
#define GPIO_DATA (0xFFFFFFFF & (~(0x0f << 4 * (GPIO_PIN_DATA-8))))
#define DS18B20_IO_IN()  {GPIO_WHAT->CRH&=GPIO_DATA;GPIO_WHAT->CRH|=8<<4*(GPIO_PIN_DATA-8);}
#define DS18B20_IO_OUT() {GPIO_WHAT->CRH&=GPIO_DATA;GPIO_WHAT->CRH|=3<<4*(GPIO_PIN_DATA-8);}
#endif

    	
u8 DS18B20_Init(void);
float DS18B20_Get_Temp(void); 
u8 DS18B20_Get_Temp_Raw(short *raw); 
void DS18B20_Start(void); 
void DS18B20_Write_Byte(u8 dat); 
u8 DS18B20_Read_Byte(void); 
u8 DS18B20_Read_Bit(void); 
u8 DS18B20_Check(void); 
void DS18B20_Rst(void); 
#endif
