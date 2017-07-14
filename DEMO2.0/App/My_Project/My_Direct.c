#include "My_Direct.h"


float DP_A = 3.0,DP_B = 3.0,DP_C = 3.0;   //舵机动态P的参数
float DD_A = 3.0;
float Error_Pwm = 0;
int Pwm_mid = 1715;
int Pwm_out = 0;
float  Direct_P = 2.9;
float  Direct_D = 0.5;



void Direct_PD(void)
{  
  float Middle_Emin = 1;
  float  Middle_Enow = 0;
  static float Middle_Elast = 0;
  
  uint8 Duoji_DividingLine1 = 30;    //二次曲线和定值P的分界线
  uint8 Duoji_DividingLine2 = 50;    //二次曲线和定值P的分界线
  
  int Pwm_max = 1890;
  int Pwm_min = 1540;
  
  
  
  
  float Pwm_Var = 0;
  float P_Var = 0;
  float D_Var = 0;
  
  Middle_Enow = MiddleError;
  Pwm_out = 1715;
/*
  if(AbsMiddleError <= Duoji_DividingLine1)
  {
     Direct_P = DP_A;
  }
  else if(AbsMiddleError > Duoji_DividingLine1 && AbsMiddleError <= Duoji_DividingLine2)
  {
    Direct_P = DP_B;
  }
  else if(AbsMiddleError > Duoji_DividingLine2)
  {
    Direct_P = DP_C;
  }    
  Direct_D = DD_A;*/
  
  if(Myabsfloat(Middle_Enow - Middle_Elast) > 80)
  {
    Middle_Enow = Middle_Elast;
  }
  
  P_Var = Direct_P*Middle_Enow;
  D_Var = Direct_D*(Middle_Enow - Middle_Elast);
  
  Middle_Elast = Middle_Enow;
  
  Pwm_Var = P_Var + D_Var;
  
  if(AbsMiddleError <= Middle_Emin)
  {
    Pwm_Var = 0;
  }
  if(Pwm_Var > 175)
  {
    Pwm_Var = 175;
  }
  if(Pwm_Var < -175)
  {
    Pwm_Var = -175;
  }
  Error_Pwm = Pwm_Var;
  
  Pwm_out = Pwm_mid + (int)Pwm_Var;
  
  if(Pwm_out >= Pwm_max)
     Pwm_out = Pwm_max;
  if(Pwm_out <= Pwm_min)
  {
    Pwm_out = Pwm_min;
  } 
  ftm_pwm_duty(FTM3,FTM_CH1,Pwm_out);
}

/*
void Duoji()
{

    if(AbsMiddleError<=Duoji_DividingLine)
    {
      Duoji_P=DP_C/10.0;
    }
    else
    {
      Duoji_P=(AbsMiddleError*AbsMiddleError)*(DP_A/10000.0)+DP_C/10.0;
    }
      if(Duoji_P>50)
        Duoji_P=50;
    
      UP=Duoji_P*MiddleError;
      
    Derivative_Now  = MiddleError - MiddleError_Last;
    Derivative_Last = MiddleError_Last - MiddleError_Previous;
    
    UD=Duoji_D*Derivative_Now*Duoji_D_A/10.0+Duoji_D*Derivative_Last*(1.0-Duoji_D_A/10.0);
   
    MiddleError_Previous = MiddleError_Last;
    MiddleError_Last = MiddleError; 
    //P项限幅，要实现提前入弯需要增大P，然而过大的P在出弯时易转过头。
    //因为P项实在过大，导致微分对舵机回正的贡献不起作用，舵机依旧是打死状态，然后就转过头了。
    if(UP>Dj_Right-Dj_Mid)
      UP = Dj_Right-Dj_Mid;
    if(UP<(Dj_Left-Dj_Mid))
      UP = Dj_Left-Dj_Mid;
    if(UD>700)
       UD = 700;
    if(UD<-700)
      UD = -700;
 
    if(MiddleError<0)  //左转
      PWM_Duoji=(int)(Dj_Mid+UP+UD);
    else 
      PWM_Duoji=(int)(Dj_Mid+Kp_DuojiCorrect/100.0*(UP+UD));
 
  if(PWM_Duoji<Dj_Left)
    PWM_Duoji=Dj_Left;
  if(PWM_Duoji>Dj_Right)
    PWM_Duoji=Dj_Right;
  int YuZhi = 150;
  if((CarState==CarStart) || (CarState == CarStop && ImgStopNum !=0 && ImgNum-ImgStopNum<YuZhi)) 
    ftm_pwm_duty(FTM2, FTM_CH0,PWM_Duoji);
  if((CarState==CarStop && ImgStopNum == 0) ||(CarState == CarStop && ImgNum-ImgStopNum>=YuZhi)) 
    ftm_pwm_duty(FTM2, FTM_CH0,Dj_Mid);

}*/
