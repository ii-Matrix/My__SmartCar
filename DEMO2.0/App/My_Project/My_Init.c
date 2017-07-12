#include "My_Init.h"


/********
所有函数模块的初始化
*********/

void Init_All(void)
{
    //电机初始化
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
  
     //舵机的初始化
     ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,1715);      //初始化 舵机 PWM
  
     //系统板LED初始化，用于程序调试
     led_init(LED_MAX);
     
    //拨码开关初始化，设置速度模式
     Switch_Init();
    
     //LCD初始化
      LCD_init();
     
      //梯形校正表及相关参数初始化
      Adjust_init();
      
      //PIT初始化,1ms                                                                                       
      pit_init_ms(PIT1, 1);                                
      set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);     
      enable_irq (PIT1_IRQn); 
      
      //串口初始化
     uart_init (UART3,115200);    //图像传输
      uart_init (UART1,9600);      //手机调试
    
      //正交解码测速
    ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
      
    
    
    //摄像头初始化
     camera_init(ImgNow);
     
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler
     
    NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    
     NVIC_SetPriority(PORTA_IRQn,0);
     NVIC_SetPriority(DMA0_IRQn,1); 
     NVIC_SetPriority(PIT1_IRQn,2);  
      
     LCD_clear(BLACK);                                        //清屏
 SetZero();                                     //参数清零或其他初值     
}