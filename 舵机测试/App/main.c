 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,ɽ��Ƽ�
  *     All rights reserved.
  *     �������ۣ�ɽ����̳ http://www.vcan123.com
  *
  *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
  *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
  *
  * @file       main.c
  * @brief      ɽ��K60 ƽ̨������
  * @author     ɽ��Ƽ�
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
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� S3010��� ����ʵ�� ,ɽ�� �ṩ�� �������ģ�飬�ܽŶ����� MOS �ܸ��룬�ź��Ƿ���ġ�
                ���糣��� �ߵ�ƽ ʹ��
                ������ϣ��뿴�������ӣ�
                http://www.vcan123.com/forum.php?mod=collection&action=view&ctid=25
 */
void main()
{
   
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,1715);      //��ʼ�� ��� PWM
    
    ftm_pwm_init(FTM0 , FTM_CH2, 10000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH3, 10000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH4, 10000,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
    ftm_pwm_init(FTM0 , FTM_CH5, 10000,0); 
    
    uart_init (UART1, 9600);  
    
    

    while(1)
    {
      LY_PID();
     ftm_pwm_duty(S3010_FTM, S3010_CH,Pwm_Value);
      
      if(HX_flag0 == 1)               //����Թ�ȫΪ�㼴��������⵽�����ߵı�־λ
           {
             ftm_pwm_duty(FTM0 , FTM_CH2,0);
             ftm_pwm_duty(FTM0 , FTM_CH3,0);
             ftm_pwm_duty(FTM0 , FTM_CH4,0);
             ftm_pwm_duty(FTM0 , FTM_CH5,0);
           }
             
            
           else
           {   
               ftm_pwm_duty(FTM0 , FTM_CH4,0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
               ftm_pwm_duty(FTM0 , FTM_CH5,2000);
               ftm_pwm_duty(FTM0 , FTM_CH2,2000);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
               ftm_pwm_duty(FTM0 , FTM_CH3, 0);                        //���ռ�ձ����ó�ʼ��   MOD =313 ;  ռ�ձ� = duty /(MOD +1 ) ;FTM_CnV_REG(FTMx[ftmn], ch) = cv;
              
           }
      
      

            
    }

}
/*******��������PID**************/
void LY_PID(void)
{      
  
  uint32 i = 0;
  char str[10];
  uint8 str1[10];
  
                //��ѯ�Ƿ���յ��ַ���
         i = uart_querystr (UART1,str,sizeof(str)-1);
        if(i)                //��ѯ�Ƿ���յ��ַ���
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
               
               
               
              uart_putstr (UART1,str1 );                          //�����ַ���
               
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
               
               
               
              uart_putstr (UART1,str1 );                          //�����ַ���
               
               
               
             }
             
             
             if(str[0] == 'a')
             {
                    i = 0;
                    HX_flag0 = 1;
                    
                    
                    // printf("off\n");
               uart_putstr (UART1, "off\n");                          //�����ַ���   
              }     
          
             if(str[0] == 'b')
             {
                    i = 0;
                    HX_flag0 = 0;
                    
                    
                    //printf("on\n");
               uart_putstr (UART1, "on\n");                          //�����ַ���   
              }    
             
           
        }   

  
  
  
}