#include "My_Init.h"


/********
���к���ģ��ĳ�ʼ��
*********/

void Init_All(void)
{
    //�����ʼ��
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
  
     //����ĳ�ʼ��
     ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,1715);      //��ʼ�� ��� PWM
  
     //ϵͳ��LED��ʼ�������ڳ������
     led_init(LED_MAX);
     
    //���뿪�س�ʼ���������ٶ�ģʽ
     Switch_Init();
    
     //LCD��ʼ��
      LCD_init();
     
      //����У������ز�����ʼ��
      Adjust_init();
      
      //PIT��ʼ��,1ms                                                                                       
      pit_init_ms(PIT1, 1);                                
      set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);     
      enable_irq (PIT1_IRQn); 
      
      //���ڳ�ʼ��
     uart_init (UART3,115200);    //ͼ����
      uart_init (UART1,9600);      //�ֻ�����
    
      //�����������
    ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
      
    
    
    //����ͷ��ʼ��
     camera_init(ImgNow);
     
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
     
    NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    
     NVIC_SetPriority(PORTA_IRQn,0);
     NVIC_SetPriority(DMA0_IRQn,1); 
     NVIC_SetPriority(PIT1_IRQn,2);  
      
     LCD_clear(BLACK);                                        //����
 SetZero();                                     //���������������ֵ     
}