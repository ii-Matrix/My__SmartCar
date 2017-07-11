 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,山外科技
  *     All rights reserved.
  *     技术讨论：山外论坛 http://www.vcan123.com
  *
  *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
  *     修改内容时必须保留山外科技的版权声明。
  *
  * @file       main.c
  * @brief      山外K60 平台主程序
  * @author     山外科技
  * @version    v5.0
  * @date       2013-08-28
  */

#include "common.h"
#include "include.h"



#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH1
#define S3010_HZ    (100)
uint16  Pwm_Value = 1700;
uint8 HX_flag0 = 1;
void LY_PID(void);


/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 S3010舵机 测试实验 ,山外 提供的 电机驱动模块，管脚都加入 MOS 管隔离，信号是反相的。
                例如常规的 高电平 使能
                舵机资料，请看如下链接：
                http://www.vcan123.com/forum.php?mod=collection&action=view&ctid=25
 */
void main()
{
   
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,1715);      //初始化 舵机 PWM
    
    ftm_pwm_init(FTM0 , FTM_CH2, 10000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH3, 10000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH4, 10000,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH5, 10000,0); 
    
    uart_init (UART1, 9600);  
    
    

    while(1)
    {
      LY_PID();
     ftm_pwm_duty(S3010_FTM, S3010_CH,Pwm_Value);
      
      if(HX_flag0 == 1)               //红外对管全为零即出界或则检测到起跑线的标志位
           {
             ftm_pwm_duty(FTM0 , FTM_CH2,0);
             ftm_pwm_duty(FTM0 , FTM_CH3,0);
             ftm_pwm_duty(FTM0 , FTM_CH4,0);
             ftm_pwm_duty(FTM0 , FTM_CH5,0);
           }
             
            
           else
           {   
               ftm_pwm_duty(FTM0 , FTM_CH4,0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
               ftm_pwm_duty(FTM0 , FTM_CH5,2000);
               ftm_pwm_duty(FTM0 , FTM_CH2,2000);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
               ftm_pwm_duty(FTM0 , FTM_CH3, 0);                        //电机占空比设置初始化   MOD =313 ;  占空比 = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
              
           }
      
      

            
    }

}
/*******蓝牙调试PID**************/
void LY_PID(void)
{      
  
  uint32 i = 0;
  char str[10];
  uint8 str1[10];
  
                //查询是否接收到字符串
         i = uart_querystr (UART1,str,sizeof(str)-1);
        if(i)                //查询是否接收到字符串
        { 
           
             if(str[0] == '0')
             {
               i = 0;
               Pwm_Value++;
               //printf("%d\n",Z_P);
               str1[0] = Pwm_Value/1000+48;
               str1[1] = Pwm_Value%1000/100+48;
               str1[2] = Pwm_Value%100/10+48;
               str1[3] = Pwm_Value%10/1+48;
                str1[4] = '\n';
               
               
               
              uart_putstr (UART1,str1 );                          //发送字符串
               
             }
             
             if(str[0] == '1')
             {
               i = 0;
               Pwm_Value--;
            
               //printf("%d\n",W_P);
               str1[0] = Pwm_Value/1000+48;
               str1[1] = Pwm_Value%1000/100+48;
               str1[2] = Pwm_Value%100/10+48;
               str1[3] = Pwm_Value%10/1+48;
               str1[4] = '\n';
               
               
               
              uart_putstr (UART1,str1 );                          //发送字符串
               
               
               
             }
             
             
             if(str[0] == 'a')
             {
                    i = 0;
                    HX_flag0 = 1;
                    
                    
                    // printf("off\n");
               uart_putstr (UART1, "off\n");                          //发送字符串   
              }     
          
             if(str[0] == 'b')
             {
                    i = 0;
                    HX_flag0 = 0;
                    
                    
                    //printf("on\n");
               uart_putstr (UART1, "on\n");                          //发送字符串   
              }    
             
           
        }   

  
  
  
}