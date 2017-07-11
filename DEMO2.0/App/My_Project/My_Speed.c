#include "My_Speed.h"

int SpeedMax = 5000;                    //����ٶ�
int SpeedMid = 4000;                    //�м��ٶ�
int SpeedMin = 3000;                    //��С�ٶ�
int SpeedGoal = 4000;                   //Ŀ���ٶ�

int  Speed_P = 0;                       //�ٶ�P��
int  Speed_I = 0;                       //�ٶ�I��

float Motor_P = 2.0;                    //���P����
float Motor_I = 3.0;                    //���I����
float Speed_State = 0.88;


int MotorOutMax=9999;                //���������PWM
int MotorOutMin=0;                  //�����С���PWM

int SpeedNow1 = 0;                 //��ǰ�ٶ�
int Speed_Test1 = 0;
int Speed_error1 = 0;
int Speed_last_error1 = 0;
int Speed_second_error1 = 0;


int PWM_Motor1 = 2000;                  //���PWM1
int PWM_Motor2 = 2000;                  //���PWM2
int Pwm_Out = 0;
int Speed_flag = 0;


int CarTime;                    //��������һȦ��ʱ��
int SpeedAverage;               //һȦ������ƽ���ٶ�


float Distance,Distance_Last;                 //·�̣���λ������
float Distance_StartLine;       //������·�̼�¼
float Distance_GoOutBend;       //����·�̼�¼
float Distance_GoInBend;
float Distance_Cross,Distance_Cross_GoOut;

long Count_Total,Count_Total_Last;               //�����ۼ�ֵ



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
        SpeedNow1=6*(Speed_Test1);
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
    
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,PWM_Motor1);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_Motor2);
    
 
}
void MotorOut2(void)
{  
    
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
  
  if(AbsMiddleError < 10)
  {
    PWM_Motor1 =  Pwm_Out;
    PWM_Motor2 =  Pwm_Out;
    Speed_flag = 0;
  }
  else 
  {
    PWM_Motor1 = (int)(Pwm_Out*(1 + Speed_State*Error_Pwm/180.0));
    PWM_Motor2 = (int)(Pwm_Out*(1 - Speed_State*Error_Pwm/180.0));
     Speed_flag = 1;   
  }
  
  if(Speed_flag == 1)
  {
    if(PWM_Motor1 >= 9999)
    {
      PWM_Motor1 = 9999;
      PWM_Motor2 = (int)(9999/(1 + (Speed_State*Error_Pwm/180.0))*(1 - (Speed_State*Error_Pwm/180.0)));
    }
    else if(PWM_Motor2 >= 9999)
    {
      PWM_Motor1 = (int)(9999/(1 - (Speed_State*Error_Pwm/180.0))*(1 + (Speed_State*Error_Pwm/180.0)));
      PWM_Motor2 = 9999;
    }
    else if(PWM_Motor1 <= 0)
    {
      PWM_Motor1 = 0;
      PWM_Motor2 = (int)( 0 - 9999*(Speed_State*Error_Pwm/180.0));
    }
    else if(PWM_Motor2 <= 0)
     {
      PWM_Motor1 =(int)( 0 + 9999*(Speed_State*Error_Pwm/180.0));
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
  Count_Total+=SpeedNow1;
  Distance = (float)Count_Total/67.0;
}