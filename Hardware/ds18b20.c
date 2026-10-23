#include "ds18b20.h"
#include "delay.h"	
 



void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); 	
    DS18B20_DQ_OUT=0; 	
    delay_us(750);    
    DS18B20_DQ_OUT=1; 	
	delay_us(15);     	
}

u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
	DS18B20_IO_OUT();	
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();	
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();	
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}

void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	
    DS18B20_Write_Byte(0x44);	
} 

 	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(GPIO_CLOCK, ENABLE);	 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_PIN;				
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIO_WHAT, &GPIO_InitStructure);

 	GPIO_SetBits(GPIO_WHAT,GPIO_PIN);  

	DS18B20_Rst();

	return DS18B20_Check();
}  
 
u8 DS18B20_Get_Temp_Raw(short *raw)
{
	u8 TL,TH;
	short tem;
	
	if(DS18B20_Init())return 1;		 
	DS18B20_Start();				 
	delay_ms(800);					 
	
	DS18B20_Rst();
	if(DS18B20_Check())return 1;
	DS18B20_Write_Byte(0xcc);	 
	DS18B20_Write_Byte(0xbe);		  
	TL=DS18B20_Read_Byte(); 		   
	TH=DS18B20_Read_Byte(); 		  
	
	tem=TH; 						 
	tem<<=8;    
	tem+=TL;						 
	
	 
	if(tem==0xFFFF||tem==0x0000)return 1;
	
	*raw=tem;
	return 0;
}
 
float DS18B20_Get_Temp(void)
{
	short tem;
	float tem_float;
	
	if(DS18B20_Get_Temp_Raw(&tem))return -1000.0f;	 
	
 
	tem_float=(float)tem*0.625f;	 
	if(tem_float>=0)tem_float+=0.5f;	 
	else tem_float-=0.5f;
	tem=(short)tem_float;
	
	return (float)tem/10.0f; 	 
}
