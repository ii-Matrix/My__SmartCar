#include "My_Debug.h"

uint8  CarState  =  0;
int   SaiDao_Flag = 1;


void Switch_Init(void)
{
  gpio_init (Switch_1, GPI,0);
  port_init_NoALT(Switch_1, PULLUP);         //����Ϊ����
  gpio_init (Switch_2, GPI,0);
  port_init_NoALT(Switch_2, PULLUP);         //����Ϊ����
  gpio_init (Switch_3, GPI,0);
  port_init_NoALT(Switch_3, PULLUP);         //����Ϊ����
  gpio_init (Switch_4, GPI,0);
  port_init_NoALT(Switch_4, PULLUP);         //����Ϊ���� 

}

void My_Switch_Set(void)
{
  
  int Switch_Flag1  = 1;
  int Switch_Flag2  = 1;
  int Switch_Flag3  = 1;
  int Switch_Flag4  = 1;
  
  Switch_Flag1 = PTE27_IN;
  Switch_Flag2 = PTE26_IN;
  Switch_Flag3 = PTE25_IN;
  Switch_Flag4 = PTE24_IN;
  
  if(Switch_Flag1 == 0)
  {
    SaiDao_Flag = 0;
    
  }
  else
  {
    SaiDao_Flag = 1;
  }
  if(Switch_Flag2 == 0)
  {
    CarState = Car_Start;
    SpeedMid = 2200; 
    SpeedGoal  = 2200;
    Speed_State = 0.28;
     Direct_P = 2.7;
  }
  if(Switch_Flag3 == 0)
  {
      CarState = Car_Start;
      SpeedMid = 2400; 
      SpeedGoal  = 2200;
      Speed_State = 0.27;
      Direct_P = 2.8;
  }
  if(Switch_Flag4 == 0)
  {
     CarState = Car_Start;
    SpeedMid = 2600; 
     SpeedGoal  = 2600;
    Speed_State = 0.30;
     Direct_P = 2.8;
  }
  
  
  
  
  
  
  
  
}



void My_Bluetooth(void)
{
  uint32 i = 0;
  int temp = 0;
  char str[10];
  char str1[10];
  static  int  Send_Data = 0;
  
             if(Send_Data == 1)
             {
              /*  str1[0] =   Speed_Test1/1000 + 48;
                str1[1] =  Speed_Test1%1000/100 + 48;
                str1[2] =   Speed_Test1%100/10 + 48;
                str1[3] =   Speed_Test1%10 + 48;
                str1[4] =  '\n';
                 str1[5] = '\0';
                uart_putstr (UART1, " Speed_Test1 ="); 
               uart_putstr (UART1, str1);*/
               vcan_sendimg(ImgNow,CAMERA_SIZE);

             }
  
          //��ѯ�Ƿ���յ��ַ���
      i = uart_querystr (UART1,str,sizeof(str)-1);
      if(i)
      {
         if(str[0] == '0')
             {
               i = 0;
               Direct_P  = Direct_P + 0.1;
               temp = (int)(Direct_P*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
               str1[4] = '\0';
               uart_putstr(UART1, "DP="); 
               uart_putstr(UART1, str1);  
               
             }
         if(str[0] == '1')
             {
               i = 0;
               Direct_P = Direct_P - 0.1;
               temp = (int)(Direct_P*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "DP="); 
               uart_putstr (UART1, str1);  
             }
          if(str[0] == '2')
             {
               i = 0;
               
               Direct_D += 0.1;
               temp = (int)(Direct_D*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "DD ="); 
               uart_putstr (UART1, str1);    //���D+
               
          /*     Motor_P += 0.1;
               temp = (int)(Motor_P*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "Motor_P ="); 
               uart_putstr (UART1, str1);*/
             }
          if(str[0] == '3')
             {
               i = 0;
              
              Direct_D -= 0.1;
               temp = (int)(Direct_D*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "DD = "); 
               uart_putstr (UART1, str1);   //���D-
               
            /*     Motor_P -= 0.1;
               temp = (int)(Motor_P*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "Motor_P ="); 
               uart_putstr (UART1, str1);*/
               
             }
          if(str[0] == '4')
             {
               i = 0;
               
                SpeedMid += 200;
                str1[0] =  SpeedMid/1000 + 48;
                str1[1] =  SpeedMid%1000/100 + 48;
                str1[2] =  SpeedMid%100/10 + 48;
                str1[3] =  SpeedMid%10 + 48;
                str1[4] =  '\n';
                str1[5] = '\0';
                uart_putstr (UART1, "SpeedMid ="); 
               uart_putstr (UART1, str1);
               
               
               
               
             /*   PWM_Motor1 += 200;
                str1[0] =  PWM_Motor1/1000 + 48;
                str1[1] =  PWM_Motor1%1000/100 + 48;
                str1[2] =  PWM_Motor1%100/10 + 48;
                str1[3] =  PWM_Motor1%10 + 48;
                str1[4] =  '\n';
                 str1[5] = '\0';
                uart_putstr (UART1, "PWM_Motor ="); 
               uart_putstr (UART1, str1);*/
               
             }
          if(str[0] == '5')
             {
               i = 0;
               
               SpeedMid -= 200;
                str1[0] =  SpeedMid/1000 + 48;
                str1[1] =  SpeedMid%1000/100 + 48;
                str1[2] =  SpeedMid%100/10 + 48;
                str1[3] =  SpeedMid%10 + 48;
                str1[4] =  '\n';
                 str1[5] = '\0';
                uart_putstr (UART1, "SpeedMid ="); 
               uart_putstr (UART1, str1);
               
               
               
               
               
               
               
            /*    PWM_Motor1 -= 200;
                str1[0] =  PWM_Motor1/1000 + 48;
                str1[1] =  PWM_Motor1%1000/100 + 48;
                str1[2] =  PWM_Motor1%100/10 + 48;
                str1[3] =  PWM_Motor1%10 + 48;
                str1[4] =  '\n';
                 str1[5] = '\0';
                uart_putstr (UART1, "PWM_Motor ="); 
               uart_putstr (UART1, str1);*/
             }
          if(str[0] == '6')
             {
               i = 0;
              // Send_Data = 1;

                Motor_I += 0.1;
               temp = (int)(Motor_I*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "Motor_I ="); 
               uart_putstr (UART1, str1);
             }
          if(str[0] == '7')
             {
               i = 0;
              // Send_Data = 0;

               Motor_I -= 0.1;
               temp = (int)(Motor_I*10.0);
               str1[0] = temp/10+48;
               str1[1] = '.';
               str1[2] = temp%10+48;
               str1[3] = '\n';
                str1[4] = '\0';
                uart_putstr (UART1, "Motor_I ="); 
               uart_putstr (UART1, str1);
             }
          if(str[0] == '8')
             {
               i = 0;
               
               Speed_State += 0.01;
               temp = (int)(Speed_State*100.0);
               str1[0] = '0';
               str1[1] = '.';
               str1[2] = temp/10+48;
               str1[3] = temp%10+48;
               str1[4] = '\n';
                str1[5] = '\0';
                uart_putstr (UART1, "Speed_State ="); 
               uart_putstr (UART1, str1);
             }
          if(str[0] == '9')
             {
               i = 0;
               
               Speed_State -= 0.01;
               temp = (int)(Speed_State*100.0);
               str1[0] = '0';
               str1[1] = '.';
               str1[2] = temp/10+48;
               str1[3] = temp%10+48;
               str1[4] = '\n';
                str1[5] = '\0';
                uart_putstr (UART1, "Speed_State ="); 
               uart_putstr (UART1, str1);
             }
          if(str[0] == 'a')
             {
               i = 0;
               CarState = Car_Stop;
               uart_putstr (UART1, "off\n");                          //�����ַ��� 
             }
          if(str[0] == 'b')
             {
               i = 0;
               CarState = Car_Start;
                uart_putstr (UART1, "on\n");                          //�����ַ���  
             }
        
        
        
      }
  
  
  
  
  
  
}
/****************************
���ã���������
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


