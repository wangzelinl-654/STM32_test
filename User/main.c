 #include "stm32f10x.h"                 
#include "Delay.h"
#include "OLED.h"
#include "sys.h"
#include "ds18b20.h"
#include <stdio.h>
#include <string.h>	
#include "PWM.h"


char oled_str[50];

                    // OLED显示缓存

int main(void)
{
    // 1. 初始化外设
    OLED_Init();                        // OLED初始化
    OLED_Clear(0);                      // 清屏
    DS18B20_Init();                     // DS18B20初始化
    PWM_Init();                         // 风扇PWM初始化（PA6 / TIM3_CH1）
    
    delay_ms(500);                      // 等待系统稳定
    
    while(1)
    {
        // 2. 读取温度（注意：DS18B20_Get_Temp内部已有800ms延时）
        float temp = DS18B20_Get_Temp(); 
        
        // 3. 显示温度到OLED
        sprintf(oled_str, "Temp: %0.1f C", temp);
        OLED_ShowString(0, 0, (u8*)oled_str, 16); // 显示温度
        
        // 4. 温控逻辑（滞回控制）
        if(temp > 30.0f) 
        {
            // 温度 > 30℃，风扇全速
            PWM_SetCompare1(100); // 100% 占空比
            OLED_ShowString(0, 2, (u8*)"Fan: FULL", 16);
        }
        else if(temp < 28.0f) 
        {
            // 温度 < 28℃，风扇停转
            PWM_SetCompare1(0);   // 0% 占空比
            OLED_ShowString(0, 2, (u8*)"Fan: OFF ", 16);
        }
        else 
        {
            // 28℃ ~ 30℃ 之间，线性调速（简单映射）
            // 温度每升高0.1度，占空比增加5%
            uint16_t duty = (uint16_t)((temp - 28.0f) * 50); // 温度差 * 50
            if(duty > 100) duty = 100;
            PWM_SetCompare1(duty);
            OLED_ShowString(0, 2, (u8*)"Fan: AUTO", 16);
        }
        
        // 5. 稍作延时，避免OLED刷屏太快（DS18B20已花了800ms）
        delay_ms(200); 
    }
}
