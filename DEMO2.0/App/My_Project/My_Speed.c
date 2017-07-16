#include "My_Speed.h"

uint16  SpeedMax = 3000;                    //����ٶ�
uint16  SpeedMid = 2400;                    //�м��ٶ�
uint16  SpeedMin = 1800;                    //��С�ٶ�
uint16  SpeedGoal = 2000;                   //Ŀ���ٶ�

float  Speed_P = 0;                       //�ٶ�P��
float  Speed_I = 0;                       //�ٶ�I��
float  Speed_D = 0;

float Motor_P = 3.0;                    //���P����
float Motor_I = 0.5;                    //���I����
float Motor_D = 0;

float Speed_State = 0.28;


int MotorOutMax=9000;                //���������PWM
int MotorOutMin=1000;                  //�����С���PWM

int SpeedNow1 = 0;                 //��ǰ�ٶ�
int Speed_Test1 = 0;
int Speed_error1 = 0;
int Speed_last_error1 = 0;
int Speed_second_error1 = 0;


int PWM_Motor1 = 1000;                  //���PWM1
int PWM_Motor2 = 1000;                  //���PWM2
int Pwm_Out = 0;
int Speed_flag = 0;


int CarTime = 0;                    //��������һȦ��ʱ��
float SpeedAverage = 0;               //һȦ������ƽ���ٶ�


float Distance = 2,Distance_Last = 2;                 //·�̣���λ������
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
 /*
    if(CurveType == Straight)
      SpeedGoal = SpeedMax;
    else if ( CurveType == Bend )
      SpeedGoal = SpeedMin;
    else
      SpeedGoal = SpeedMid;
   
     if(SpeedGoal<SpeedMin)
       SpeedGoal = SpeedMin;*/
  
  if(RoadType == Normal)
  {
    SpeedGoal = SpeedMid;
  }
  else if(RoadType == Straight1)
  {
    SpeedGoal = SpeedMax;
  }
  else if(RoadType == Ring)
  {
    SpeedGoal = SpeedMin;
  }
  else 
  {
    SpeedGoal = SpeedMid;
  }
    
 
     

}
//�ٶ��趨2
void SpeedSet_2(void)
{
  if(AbsMiddleError < 5)
  {
    SpeedGoal = SpeedMid + 300;
  }
  
  else if(AbsMiddleError >5 && AbsMiddleError <15)
  {
    SpeedGoal = SpeedMid;
  }
  else if(AbsMiddleError >15 && AbsMiddleError <40)
  {
    SpeedGoal = SpeedMid - 10*AbsMiddleError;
  }
  else 
  {
     SpeedGoal = SpeedMid - 500;
  }
  
  if(RoadType == Ring)
  {
    if(SpeedNow1 > SpeedMin)
    {
      SpeedGoal = 0;
    }
    else
     SpeedGoal = SpeedMin;
  }
  
       if(SaiDao_Flag == 1)
    {
        if(Distance > 3050)
          {
         if(SpeedNow1 > 1000)
         SpeedGoal = 0;
         else
           SpeedGoal = 2200;
        
         
         }
    }
    else if(SaiDao_Flag == 0)
    {
      if(Distance < 500)
          {
      
         SpeedGoal = 1000;
        
         }
      else
      {
         SpeedMid = 2400;
         
      }
      
    }
  
 
  
}
/********************************
���ã��ɼ��������ķ������ٶ�
��������
********************************/
void Speed_Measure(void)
{
        Speed_Test1 = ftm_quad_get(FTM1);
    // Speed_test1 = -Speed_test1;
       if(Speed_Test1 < 0)
       {
         Speed_Test1 = -Speed_Test1;
       }
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
   
  // SpeedSet();
   SpeedSet_2();
   
   // Speed_PID();
    Speed_PID_2();
    
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,PWM_Motor1 );
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_Motor2);
    
 
}
void MotorOut2(void)
{  
     Speed_Measure();
     DistanceRecord();
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,SpeedGoal);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,SpeedGoal);
    
 
}
/****************************
���ã�����ʽPID
****************************/
void Speed_PID()    
{
  Speed_second_error1 = Speed_last_error1;    
  Speed_last_error1 =   Speed_error1;
  Speed_error1 =  SpeedGoal - SpeedNow1;
    
  
  
  Speed_P = Motor_P*(float)(Speed_error1 - Speed_last_error1);
  Speed_I = Motor_I*(float)Speed_error1;

  if(abs(Speed_error1)>1)                                            //��������
    Pwm_Out += (Speed_P + Speed_I);
  /*  if(Pwm_Out > MotorOutMax)      
       Pwm_Out = MotorOutMax;
  if(Pwm_Out < MotorOutMin)
    Pwm_Out = MotorOutMin;*/
  
  if(AbsMiddleError < 5.0)
  {
    PWM_Motor1 =  Pwm_Out;
    PWM_Motor2 =  Pwm_Out;
    Speed_flag = 0;
  }
  else 
  {
    PWM_Motor1 = (uint16)(Pwm_Out*(1 + Speed_State*Error_Pwm/175.0));
    PWM_Motor2 = (uint16)(Pwm_Out*(1 - Speed_State*Error_Pwm/175.0));
     Speed_flag = 1;   
  }
  
  if(Speed_flag == 1)
  {
    if(PWM_Motor1 >= 9999)
    {
      PWM_Motor1 = MotorOutMax;
      PWM_Motor2 = (uint16)(MotorOutMax/(1 + (Speed_State*Error_Pwm/175.0))*(1 - (Speed_State*Error_Pwm/175.0)));
    }
    else if(PWM_Motor2 >= MotorOutMax)
    {
      PWM_Motor1 = (uint16)(MotorOutMax/(1 - (Speed_State*Error_Pwm/175.0))*(1 + (Speed_State*Error_Pwm/175.0)));
      PWM_Motor2 = MotorOutMax;
    }
    else if(PWM_Motor1 <= 0)
    {
      PWM_Motor1 = 0;
      PWM_Motor2 = (uint16)( 0 - 9999*(Speed_State*Error_Pwm/175.0));
    }
    else if(PWM_Motor2 <= 0)
     {
      PWM_Motor1 =(uint16)( 0 + 9999*(Speed_State*Error_Pwm/175.0));
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

void  Speed_PID_2()
{
  Speed_second_error1 = Speed_last_error1;    
  Speed_last_error1 =   Speed_error1;
  Speed_error1 =  SpeedGoal - SpeedNow1;
  
  Speed_P = Motor_P*(float)(Speed_error1 - Speed_last_error1);
  Speed_I = Motor_I*(float)Speed_error1;
  Speed_D = Motor_D*(float)(Speed_error1 - 2*Speed_last_error1 + Speed_second_error1);
  
  
 //  if(abs(Speed_error1)>1)                                            //��������
    Pwm_Out += (int)(Speed_P + Speed_I + Speed_D);
   if(Pwm_Out > MotorOutMax)      
       Pwm_Out = MotorOutMax;
  if(Pwm_Out <MotorOutMin)
    Pwm_Out =MotorOutMin;
 
  
  if(AbsMiddleError < 5)
  {
    PWM_Motor1 =  Pwm_Out;
    PWM_Motor2 =  Pwm_Out;
    Speed_flag = 0;
  }
  else 
  {
    PWM_Motor1 = (int)(Pwm_Out*(1 - Speed_State*Error_Pwm/175.0));
    PWM_Motor2 = (int)(Pwm_Out*(1 + Speed_State*Error_Pwm/175.0));
     Speed_flag = 1;   
  }
  
  if(Speed_flag == 1)
  {
    if(PWM_Motor1 >= MotorOutMax)
    {
      PWM_Motor1 = MotorOutMax;
      PWM_Motor2 = (int)(MotorOutMax/(1 - (Speed_State*Error_Pwm/175.0))*(1 + (Speed_State*Error_Pwm/175.0)));
    }
    else if(PWM_Motor2 >= MotorOutMax)
    {
      PWM_Motor1 = (int)(MotorOutMax/(1 + (Speed_State*Error_Pwm/175.0))*(1 - (Speed_State*Error_Pwm/175.0)));
      PWM_Motor2 = MotorOutMax;
    }
    else if(PWM_Motor1 <=MotorOutMin)
    {
      PWM_Motor1 = MotorOutMin;
      PWM_Motor2 = (int)(MotorOutMin + MotorOutMax*(Speed_State*Error_Pwm/175.0));
    }
    else if(PWM_Motor2 <= MotorOutMin)
     {
      PWM_Motor1 =(int)(MotorOutMin- MotorOutMax*(Speed_State*Error_Pwm/175.0));
      PWM_Motor2 = 0;
    }
  }
  else if(Speed_flag == 0)
  {
    
    
           PWM_Motor1 = (PWM_Motor1 >= MotorOutMax)?MotorOutMax:PWM_Motor1;
           PWM_Motor2 = (PWM_Motor2 >= MotorOutMax)?MotorOutMax:PWM_Motor2;
           
            
           PWM_Motor1 = (PWM_Motor1 <= MotorOutMin)?MotorOutMin:PWM_Motor1;
           PWM_Motor2 = (PWM_Motor2 <= MotorOutMin)?MotorOutMin:PWM_Motor2;
           
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