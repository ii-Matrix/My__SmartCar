#include "My_IRQ.h"

uint8 KeyTime=0;
uint8 SpeedTime=0;
uint8 PhotoTime = 0;


/*********************
���ã���ʱ���ж�
��������
���أ���
**********************/
void PIT1_IRQHandler()
{
  KeyTime++;
  SpeedTime++;
  PhotoTime++;
  
   if(KeyTime>=10)//8ms����һ��
  {
    KeyTime=0;
    key_IRQHandler();
    My_Bluetooth();
 
  }
 

  if(SpeedTime>=5)
  {
    SpeedTime = 0;

    if(CarState == Car_Start)
    {
      // MotorOut();
          MotorOut2();
      SpeedAverage = (Distance*1000/(float)CarTime);     
    }
    if(CarState == Car_Stop)
    {
      ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);
      ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
      ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
      ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,0);
      
    }
     led_turn (LED0);
  }  

  if(CarState == Car_Start)
  {
    CarTime++;    
  }
  PIT_Flag_Clear(PIT1);
}





/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;  //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
    
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
    
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}
