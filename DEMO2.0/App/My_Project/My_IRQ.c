#include "My_IRQ.h"

uint8 KeyTime=0;
uint8 SpeedTime=0;
uint8 PhotoTime = 0;


/*********************
作用：定时器中断
参数：无
返回：无
**********************/
void PIT1_IRQHandler()
{
  KeyTime++;
  SpeedTime++;
  PhotoTime++;
  
   if(KeyTime>=10)//8ms调用一次
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
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;  //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
    
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
    
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}
