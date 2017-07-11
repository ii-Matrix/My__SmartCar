#include "My_Debug.h"

uint8  CarState  =  0;


void Switch_Init(void)
{
  gpio_init (Switch_1, GPI,0);
  port_init_NoALT(Switch_1, PULLUP);         //配置为上拉
  gpio_init (Switch_2, GPI,0);
  port_init_NoALT(Switch_2, PULLUP);         //配置为上拉
  gpio_init (Switch_3, GPI,0);
  port_init_NoALT(Switch_3, PULLUP);         //配置为上拉
  gpio_init (Switch_4, GPI,0);
  port_init_NoALT(Switch_4, PULLUP);         //配置为上拉 

}
void My_Bluetooth(void)
{
  uint32 i = 0;
  char str[10];
  char str1[10];
  
  
  
          //查询是否接收到字符串
      i = uart_querystr (UART1,str,sizeof(str)-1);
      if(i)
      {
         if(str[0] == '0')
             {
               i = 0;
             }
         if(str[0] == '1')
             {
               i = 0;
             }
          if(str[0] == '2')
             {
               i = 0;
             }
          if(str[0] == '3')
             {
               i = 0;
             }
          if(str[0] == '4')
             {
               i = 0;
             }
          if(str[0] == '5')
             {
               i = 0;
             }
          if(str[0] == '6')
             {
               i = 0;
             }
          if(str[0] == '7')
             {
               i = 0;
             }
          if(str[0] == '8')
             {
               i = 0;
             }
          if(str[0] == '9')
             {
               i = 0;
             }
          if(str[0] == 'a')
             {
               i = 0;
               CarState = Car_Stop;
               uart_putstr (UART1, "off\n");                          //发送字符串 
             }
          if(str[0] == 'b')
             {
               i = 0;
               CarState = Car_Start;
                uart_putstr (UART1, "on\n");                          //发送字符串  
             }
        
        
        
      }
  
  
  
  
  
  
}
/****************************
作用：参数清零
****************************/
void SetZero()
{
 
  memset(Left,Left_Init,120);
  memset(Middle,80,120);
  memset(Right,Right_Init,120);
  memset(Lost_L,0,120);
  memset(Lost_R,0,120); 
  Lost_L_Num=Lost_R_Num=0;
  Sight_Last = Sight;
  Sight=Sight_Init;
  Sight_L=Sight_L_Init;
  Sight_R=Sight_R_Init;
  Last_L=Left_Init;
  Last_R=Right_Init;
  
  
  ValidLine=0;
  
  
  if(CarState == Car_Stop)
  {
    Distance_StartLine = 0;
  }
  
}


