#include "My_Speed.h"

uint16  SpeedMax = 3000;                    //����ٶ�
uint16  SpeedMid = 3000;                    //�м��ٶ�
uint16  SpeedMin = 3000;                    //��С�ٶ�
uint16  SpeedGoal = 3000;                   //Ŀ���ٶ�

int  Speed_P = 0;                       //�ٶ�P��
int  Speed_I = 0;                       //�ٶ�I��

float Motor_P = 3.0;                    //���P����
float Motor_I = 3.0;                    //���I����
float Speed_State = 0.88;


uint16 MotorOutMax=9999;                //���������PWM
uint16 MotorOutMin=0;                  //�����С���PWM

uint16 SpeedNow1 = 0;                 //��ǰ�ٶ�
int32 Speed_Test1 = 0;
int32 Speed_error1 = 0;
int32 Speed_last_error1 = 0;
int32 Speed_second_error1 = 0;


uint16 PWM_Motor1 = 2000;                  //���PWM1
uint16 PWM_Motor2 = 2000;                  //���PWM2
uint16 Pwm_Out = 0;
int Speed_flag = 0;


uint16 CarTime = 0;                    //��������һȦ��ʱ��
uint16 SpeedAverage = 0;               //һȦ������ƽ���ٶ�


float Distance = 0,Distance_Last = 0;                 //·�̣���λ������
float Distance_StartLine = 0;       //������·�̼�¼
float Distance_GoOutBend = 0;       //����·�̼�¼
float Distance_GoInBend = 0;
float Distance_Cross = 0,Distance_Cross_GoOut = 0;

long Count_Total = 0,Count_Total_Last = 0;               //�����ۼ�ֵ



/****************************
δ��֤
���ã��ٶȿ���
****************************/
void SpeedSet(void)
{
    if(CurveType == Straight)
      SpeedGoal = SpeedMax;
    else if ( CurveType == Bend )
      SpeedGoal = SpeedMin;
    else
      SpeedGoal = SpeedMid;

     if(SpeedGoal<SpeedMin)
       SpeedGoal = SpeedMin;

}
/********************************
���ã��ɼ��������ķ������ٶ�
��������
********************************/
void Speed_Measure(void)
{
        Speed_Test1 = ftm_quad_get(FTM1);
    // Speed_test1 = -Speed_test1;
       
	ftm_quad_clean(FTM1);        
        SpeedNow1=40*(Speed_Test1);
        // printf("%d\n",Speed_Test1);
}

/****************************
���ã�������PWM
����������õ�PWM
****************************/
void MotorOut(void)
{  
  
    Speed_Measure();
    DistanceRecord();
    SpeedSet();
    Speed_PID();
    
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,PWM_Motor1);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_Motor2);
    
 
}
void MotorOut2(void)
{  
     Speed_Measure();
     DistanceRecord();
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,PWM_Motor1);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_Motor1);
    
 
}
/****************************
���ã�����ʽPID
****************************/
void Speed_PID()    
{
  Speed_second_error1 = Speed_last_error1;    
  Speed_last_error1 =   Speed_error1;
  Speed_error1 =  SpeedGoal - SpeedNow1;
    
  
  
  Speed_P = Motor_P*(Speed_error1 - Speed_last_error1);
  Speed_I = Motor_I*Speed_error1;

  if(abs(Speed_error1)>1)                                            //��������
    Pwm_Out += (Speed_P + Speed_I);
    if(Pwm_Out > MotorOutMax)      
       Pwm_Out = MotorOutMax;
  if(Pwm_Out < MotorOutMin)
    Pwm_Out = MotorOutMin;
  
  if(AbsMiddleError < 5)
  {
    PWM_Motor1 =  Pwm_Out;
    PWM_Motor2 =  Pwm_Out;
    Speed_flag = 0;
  }
  else 
  {
    PWM_Motor1 = (int)(Pwm_Out*(1 + Speed_State*Error_Pwm/175.0));
    PWM_Motor2 = (int)(Pwm_Out*(1 - Speed_State*Error_Pwm/175.0));
     Speed_flag = 1;   
  }
  
  if(Speed_flag == 1)
  {
    if(PWM_Motor1 >= 9999)
    {
      PWM_Motor1 = 9999;
      PWM_Motor2 = (int)(9999/(1 + (Speed_State*Error_Pwm/175.0))*(1 - (Speed_State*Error_Pwm/175.0)));
    }
    else if(PWM_Motor2 >= 9999)
    {
      PWM_Motor1 = (int)(9999/(1 - (Speed_State*Error_Pwm/175.0))*(1 + (Speed_State*Error_Pwm/175.0)));
      PWM_Motor2 = 9999;
    }
    else if(PWM_Motor1 <= 0)
    {
      PWM_Motor1 = 0;
      PWM_Motor2 = (int)( 0 - 9999*(Speed_State*Error_Pwm/175.0));
    }
    else if(PWM_Motor2 <= 0)
     {
      PWM_Motor1 =(int)( 0 + 9999*(Speed_State*Error_Pwm/175.0));
      PWM_Motor2 = 0;
    }
  }
  else if(Speed_flag == 0)
  {
    
    
           PWM_Motor1 = (PWM_Motor1 >= 9999)?9999:PWM_Motor1;
           PWM_Motor2 = (PWM_Motor2 >= 9999)?9999:PWM_Motor2;
           
            
           PWM_Motor1 = (PWM_Motor1 <= 0)?0:PWM_Motor1;
           PWM_Motor2 = (PWM_Motor2 <= 0)?0:PWM_Motor2;
           
                   if(PWM_Motor1 > PWM_Motor2)
                   {
                      PWM_Motor2 = PWM_Motor1;
                   }
                   else
                   {
                     PWM_Motor1 = PWM_Motor2;
                   }
  }
  
}
/****************************
���ã�·�̼�¼
****************************/
void DistanceRecord()
{
  Count_Total += Speed_Test1;
  Distance = (float)Count_Total/50.0;
}