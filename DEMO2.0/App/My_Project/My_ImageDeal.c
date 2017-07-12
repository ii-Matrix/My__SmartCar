#include "My_ImageDeal.h"

uint8 ImgBufferAgo[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10],ImgBufferNow[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10];//ԭʼѹ��ͼ��洢������
uint8 Img[CAMERA_H][CAMERA_W];           //��ѹ�����ͼ��                      1
uint8 *ImgNow,*ImgAgo;                //                                 1


float Img_P[12] = {1,1,1,1,1,1,1,1,1,1,1,1};                        //ͼ��Ȩ�رȣ�ÿ10��һ��Ȩ�ر�      
float a2 = 0,b2 = 0,c2 = 0,d2 = 0,e2 = 0,f2 = 0,v2 = 0;                //��У��ϵ��                      
uint8  Left[CAMERA_H],Middle[CAMERA_H],Right[CAMERA_H];//��,�У�����ֵ       1
int Last_L,Last_R;                    //��һ�еı߽�ֵ                     1
uint8 Jizhun_L[3],Jizhun_R[3];          //��׼����                         1
uint8 Lost_L[CAMERA_H],Lost_R[CAMERA_H];        //�������                   1

int Left_Init=5,Right_Init=155,Middle_Init=80;           //�����ҳ�ʼֵ����ֹ����ʱ����Խ��               1
int Sight_Last,Sight=24,Sight_L=24,Sight_R=24,Sight_Init=24,Sight_L_Init=24,Sight_R_Init=24;//ǰհ����ʼ��   1
int Sight_Init_Turn  = 26;
Road_Type RoadType,RoadType_Last;                 //��������       2
uint8 Lost_L_Num,Lost_R_Num;    //�����ۼ�
uint8 ValidLine;                //��Ч����



uint32 Flag_GoInBend;           //ֱ�������־λ
uint32 Flag_GoOutBend;          //ֱ�߳����־λ

uint32 ImgNum;                  //ͼƬ��Ŀ
uint32 ImgStopNum;              //ֹͣʱ��ͼƬID
uint32 StraightNum;             //����ֱ������Ŀ
uint32 BendNum;                 //�����������Ŀ
uint32 Img_GoOutBend;            //����ʱ��ͼƬ��Ŀ
uint8  CrossNum;                //ʮ����Ŀ




float MiddleError,MiddleError_Last,MiddleError_Previous;              //����ͼ��ƫ��,�ϳ������ϳ�
float AbsMiddleError;           //ͼ��ƫ��ľ���ֵ
float ImgCurveDegree;           //���������̶�ϵ��


Curve_Type CurveType;           //�����̶ȣ���ҪΪ���ٶȿ��Ʒ���
int Row;
float Cross_Xielv,Cross_Xielv2;


/****************************
���ã�ͼ��ײ�Ѱ�ң�ȷ����׼ֵ
****************************/
void BottomFind()
{   
  int x=0,y=0;                              //yΪ�����꣬xΪ������
  int BlackNum_L=0,BlackNum_R=0;            //�ײ��������Һڵ�����
  int Start_L = 80;
  int Start_R = 80 ;                        //�ײ�������ʼ��

 
    for(y=119;y>116;y--)                  //ͳ�����Һڵ���Ŀ������>�ң����Կ��ҿ�ʼ����
    {
      for(int x = 79;x>4;x-=4)            //-4��Ϊ�˼ӿ��ٶȣ������ж�û��Ҫ����ȷ
      {
        if(Img[y][x]==ImgBlack)
          BlackNum_L++;
      }
      for(int x = 155;x>80;x-=4)
      {
        if(Img[y][x]==ImgBlack)
             BlackNum_R++;
      }
    }
    if(BlackNum_R>BlackNum_L)
      Start_L = Start_R = 52;
    else if(BlackNum_R<BlackNum_L)
      Start_L = Start_R = 108;
    
    for(y=119;y>116;y--)
    {   
      //���һ�׼
      for(x=Start_R;x<Right_Init;x++)
      {   //Ѱ�������
        if(Img[y][x+4]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x]==ImgWhite && Img[y][x-2]==ImgWhite && Img[y][x-4]==ImgWhite && Img[y][x-6]==ImgWhite)
        {
          Right[y]=x-1;             //��¼
          Lost_R[y]=0;            //���߱�־λ����
          break;
        }
      }
      
      //�����׼
      for(x=Start_L;x>Left_Init;x--)
      {
        //Ѱ�������
        if(Img[y][x-2]==ImgBlack && Img[y][x-1]==ImgBlack && Img[y][x]==ImgWhite && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite && Img[y][x+3]==ImgWhite)
        {
          Left[y]=x+1;             //��¼
          Lost_L[y]=0;            //���߱�־λ����
          break;
        }
      }
      
      Jizhun_R[119-y] = Right[y];
      Jizhun_L[119-y] = Left[y];
      Middle[y]=(Right[y]+Left[y])/2;
      //   lvbo_Jizhun(y);
    }                       
    //�����׼��ֵ
    Last_L=(Jizhun_L[0]+Jizhun_L[1]+Jizhun_L[2])/3;
    Last_R=(Jizhun_R[0]+Jizhun_R[1]+Jizhun_R[2])/3;
}


/****************************
���ã��ҵ��߽���
****************************/
void GenZong()
{
  int x=0,y=0;
  int Window = 10;
  
  
    uint8 Final_L=Left_Init,Final_R=Right_Init,Start_L,Start_R;
    
    //һ·���ٵ���Чǰհ���¸�
    for(y=116;y>=Sight_Init;y--)// && Sight_L==Sight_L_Init && Sight_R==Sight_R_Init;y--)//�ж�����ô������Ϊ��ʱ��Ҫ�ı�Sight֮���ֵ����������
    {
        //��߽���������� 
        if(Last_L - Window > Left_Init)
          Final_L = Last_L - Window;     
       
        if(Last_L + Window < Right_Init)
          Start_L = Last_L + Window;
        else
          Start_L = Right_Init;                  //�ұ߽�
        
         Left[y]=Left_Init;                        //����ֵ���ж��Ƿ�δ�ҵ��߽�
        
        for(x=Start_L;x>Final_L;x--)     
        {
          if(Img[y][x]==ImgBlack)                        //�ҵ��ڵ�
          {                                            //��һ��ȷ��
            if(Img[y][x-1]==ImgBlack && Img[y][x-2]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite)
            { 
              Left[y] = x + 2;
              Lost_L[y]=0;                     //���߱�־λ����
              break;                          //�˳�ѭ�� 
            }
          }           
        }
        
        Last_L=Left[y];                              //������һ�ε�ֵ

        if(Left[y]==Left_Init)                    //˵��û����ȡ���߽�
        {
          Last_L=Left[y]=Refind_L(y);                //�ٴ�Ѱ��
        }
        
        //�ұ߽���������� 

        if(Last_R+Window<Right_Init)
          Final_R=Last_R+Window;
        else
          Final_R=Right_Init;
        
        if(Last_R-Window>Left_Init)
          Start_R=Last_R-Window;
        else
          Start_R=Left_Init;
        
        Right[y]=Right_Init;
          
        for(x=Start_R;x<Final_R;x++) 
        {
          if(Img[y][x]==ImgBlack)//�ҵ��ڵ�
          {//��һ��ȷ��
            if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite)
            { 
              Right[y]=x-2;
              Lost_R[y]=0;
              break;//�˳�ѭ��
            }
          }           
        }
        Last_R=Right[y];     //������һ�ε�ֵ
        if(Right[y]==Right_Init)//˵���ұ߽�û����ȡ��
        {
          Last_R=Right[y]=Refind_R(y);    
        }     
    }
    
    
}
/****************************
���ã���������
****************************/
void jisuan_Middle()
{
  int temp_x=0,temp_y=0,temp_M=0;
  uint8 flag=0; 
  int middle=79;
  
  for(int y=116;y>=10;y--)
  {
     
      if(Lost_L[y]==0 && Lost_R[y]==0)      //���߶����ҵ�
      {
        middle=(Left[y]+Right[y])/2;  
       }
    else if((0!=Lost_L[y]) && (0!=Lost_R[y]))     //���߶�û��
      {
        if((Lost_L[y]==LostWhite) && (Lost_R[y]==LostWhite))     //���߶�û��,���Ҷ����ߣ�������ʮ��
        {
         // middle=(Middle[y+1]+Middle[y+2]+Middle[y+3])/3;       //�е����ǰ���о�ֵ,����ƫ��ʱ���в�����

          
            Last_L=Left[y]=fanjiaozheng_x(y,middle,-ROAD_HALF);     //ƽ���������һ�벹�߽���
            Last_R=Right[y]=fanjiaozheng_x(y,middle,+ROAD_HALF);   //����
             middle=(Left[y]+Right[y])/2;    
           
        }
        else
          middle=(Middle[y+3]+Middle[y+2]+Middle[y+1])/3;
      }
   else if((0==Lost_L[y]) && (0!=Lost_R[y]))     //����У��ұ�û��
      { 
        Lost_R_Num++;
        if(Lost_R[y+1] ==LostWhite && Lost_L[y+1] == LostWhite)
          middle = Middle[y+1];
        else       
          
          middle=Middle[y+1]+(Left[y]-Left[y+1]);//+(int)(Lost_R_Num*Lost_R_Num*ImgCurveDegree);  //������߱仯���Ʋ���
      }
      
      else if((0!=Lost_L[y]) && (0==Lost_R[y]))     //���û�У��ұ���
      { 
        Lost_L_Num++;
        if(Lost_R[y+1] ==LostWhite && Lost_L[y+1] == LostWhite)
          middle = Middle[y+1];
        else
          middle=Middle[y+1]-(Right[y + 1]-Right[y]);//-(int)(Lost_L_Num*Lost_L_Num*ImgCurveDegree);  //�����ұ߱仯���Ʋ���   
      }
   

    if(middle>Right_Init)
      middle=Right_Init;
    if(middle<Left_Init)
      middle=Left_Init;
    Middle[y]=middle;
  }
  //  Lvbo_Middle();//���
  
  
}
/****************************
���ã�δ�ҵ��߽���ʱ�ٴ�Ѱ��
��������ǰ�е�������
���أ��ҵ��ı߽��ߺ�����
****************************/
uint8 Refind_L(uint8 y)//����һ����߽�
{
  int Final,Start,x;            //�������ֳ���һ���������͵Ŀ���������uint8��������������ܱ�159��
  
   
    uint8 Final_L=Left_Init,Final_R=Right_Init,Start_L,Start_R;
    int window = 22;
    //��߽���������� 
    if((int)Left[y+1]-window>Left_Init)
      Final_L=Left[y+1]-window;       
    if(Left[y+1]+window<Right_Init)
      Start_L=Left[y+1]+window;
    else
      Start_L=Right_Init;
    
    for(x=Start_L;x>Final_L;x--)     
    {
      if(Img[y][x]==ImgBlack)//�ҵ��ڵ�
      {//��һ��ȷ��
        if(Img[y][x-1]==ImgBlack && Img[y][x-1]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite)
        { 
          Lost_L[y]=0;        //���߱�־λ����
          return x+2;  
        }
      } 
    }          
    
    //�����Ҳ���������������ɢ����
    if(Left[y+1]<78 && Right[y+1]>82 )      //������ɢ����������������ʮ��
    {
      for(x=80;x>Left_Init;x--)     
      {
        if(Img[y][x]==ImgBlack)//�ҵ��ڵ�
        {//��һ��ȷ��
          if(Img[y][x-1]==ImgBlack && Img[y][x-2]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite && Img[y][x+3]==ImgWhite)
          { 
            Lost_L[y]=0;        //���߱�־λ����
            return x+2;            
          }           
        }
      }
    }
    
    
    if(Img[y][Left[y+1]]==ImgWhite )//�����ǰ�ɫ                          
      Lost_L[y]=LostWhite;
    
    if(Img[y][Left[y+1]]==ImgBlack)//�����Ǻ�ɫ
      Lost_L[y]=LostBlack;
    
    return Left[y+1];//����û�ҵ���������һ�е�ֵ
  

}
/****************************
���ã�δ�ҵ��߽���ʱ�ٴ�Ѱ��
��������ǰ�е�������
���أ��ҵ��ı߽��ߺ�����
****************************/
uint8 Refind_R(uint8 y)         //����һ���ұ߽�
{
  int Final_R=Right_Init,Start_R;
  uint8 Final,Start,x; 
    int window = 22;
    
    if(Right[y+1]+window<Right_Init)
      Final_R=Right[y+1]+window;
    else
      Final_R=Right_Init;
    
    if((int)Right[y+1]-window>Left_Init)
      Start_R=Right[y+1]-window;
    else
      Start_R=Left_Init;
    //�ұ߽���������� 
    for(x=Start_R;x<Final_R;x++) 
    {
      if(Img[y][x]==ImgBlack)//�ҵ��ڵ�
      {//��һ��ȷ��
        if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite)
        { 
          Lost_R[y]=0;        //���߱�־λ����
          return x-2; 
        }
      }           
    }
    
    if(Left[y+1]<78 && Right[y+1]>82 )  
      //�����Ҳ���������������ɢ����
    {
      for(int x=80;x<Right_Init;x++)
      {
        if(Img[y][x]==ImgBlack)//�ҵ��ڵ�
        {//��һ��ȷ��
          if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite && Img[y][x-3]==ImgWhite)
          { 
            Lost_R[y]=0;        //���߱�־λ����
            return x-2;
          }
        }  
      }    
    }
    if(Img[y][Right[y+1]]==ImgWhite)                                   
      Lost_R[y]=LostWhite;
    
    if(Img[y][Right[y+1]]==ImgBlack)
      Lost_R[y]=LostBlack;
    return Right[y+1];
}

/****************************
���ã���̬ǰհ��ȷ��
****************************/
void ForeSight()
{
  
    for(int y=80;y>=Sight_Init;y--)      
    { 
      //�����������㶪ʧ���Ҷ��Ǻ�ɫ�����ж���������
      // if(Img[y][Left[y]] == ImgBlack && Img[y-1][Left[y-1]] == ImgBlack && Img[y-2][Left[y-2]] == ImgBlack)
      if(Lost_L[y] == LostBlack && Lost_L[y-1] == LostBlack && Lost_L[y-2] == LostBlack)
      {
        Sight_L=y+1;
        break;
      }
      //��߽��߱ȽϿ���
      if(y<80 && Left[y]>150 && Left[y+1]>140 && Left[y]>Left[y+1])   
      {
        Sight_L=y;
        break;
      }
    }
  
  
  
    for(int y=80;y>=Sight_Init;y--)
    {   //�����������㶪ʧ���Ҷ��Ǻ�ɫ�����ж���������
      //   if(Img[y][Right[y]] == ImgBlack && Img[y-1][Right[y-1]] == ImgBlack && Img[y-2][Right[y-2]] == ImgBlack)
      if(Lost_R[y] == LostBlack && Lost_R[y-1] == LostBlack && Lost_R[y-2] == LostBlack)
      { 
        Sight_R=y+1;
        break;    
      }
      //      �ұ߽��߱ȽϿ���
      if(y<80 && Right[y]<10 && Right[y+1]<20 && Right[y]<Right[y+1])   
      {
        Sight_R=y;
        break;
      }    
    
  }

  if(Sight_L>Sight_R)   // ǰհȡ��Զ��
    Sight=Sight_R;
  else
    Sight=Sight_L;
  
}
/****************************
���ã��ж���������
****************************/
void judge_road()
{
  uint8 judge=0;        //�������������Ŀ����Ҫ���жϳ�һ���������ͺ��ٽ��������������͵��жϣ���������
 
  //�ͳ�������ͻ��Ҫ�ģ���
 // CrossCheck();                      //��ʮ�ּ��
 // judge = judge_crossxieL(judge);    //��б��ʮ��          
 // judge = judge_crossxieR(judge);    //��б��ʮ��
  
    //Ring_Deal();
     Ring_Deal_2();
  StraightCheck();                  //ֱ�߼��
  //GoInBendCheck();                  //������
  OutsideCheck();                   //������
  
}

/****************************
δ��֤
���ã�ֱ���ж�
****************************/
void StraightCheck()
{
  {   
    uint8 Yuzhi=10;
    if(AbsMiddleError<Yuzhi && Sight - Sight_Init<2)//ƫ��С��ǰհԶ
    {     
        RoadType = Straight1;
        if(Count_Total-Count_Total_Last>335)//ÿ5CM����һ��
        {
          Count_Total_Last = Count_Total;
          StraightNum++;
          BendNum=0; 
        }
       
    }
    else
    {
      RoadType = Normal;
      StraightNum = 0;
    }
      if(StraightNum>0)                //�������������ж�Ϊֱ�����Ž��������͸�ֵΪֱ��
    {                                //�Ͼ��ж�Ϊֱ���󣬵���ٶȻ�˲�ӣ�������������
       RoadType = Straight1;
    }
  }
}
/******************
******Բ���Ĵ���
***************/
void Ring_Deal()
{
  
   int Ring_num = 0;
   static int Guai_Flag = 0;
   
   for(int y=80;y>=Sight;y--)      
    {
      if(((Right[y+2] - Left[y+2]) < (Right[y] - Left[y]))&& ((Right[y] - Left[y]) < (Right[y-2] - Left[y-2]))&&((Right[y-2] - Left[y-2]) < (Right[y-4] - Left[y-4])) )
      {
         if(Right[y+2] < Right[y] && Right[y] < Right[y-2] && Right[y-2] < Right[y-4])
         {
            if(Left[y+2] > Left[y] && Left[y] > Left[y - 2]&& Left[y-2] > Left[y-4])
            {
               Ring_num++;
            }
             else
          {
             Ring_num = 0;
            }
             if(Ring_num >= 10)
     {
       RoadType = Ring;
       break;
     }
         }
         
      }
    
    
    }
     
     if((RoadType == Ring || RoadType == Right_Ring|| RoadType == Left_Ring)&&(Guai_Flag == 0))
     {
       for(int y = 119;y>=Sight;y--)
       {
        if(((Lost_R[y] == LostWhite) && (Lost_L[y] == 0))&&((Lost_R[y-1] == LostWhite) && (Lost_L[y-1] ==0))&&((Lost_R[y-2] == LostWhite) && (Lost_L[y-2] == 0)) ) 
       {
         RoadType = Right_Ring;
         for(int x = 119;x >= y;x--)
         {
           Middle[x] =  ((119 - x)*(Right[y] - Middle[119]))/(119-y);
         }
         break;
       }
       else  if(((Lost_R[y] == 0) && (Lost_L[y] == LostWhite))&&((Lost_R[y-1] == 0) && (Lost_L[y-1] == LostWhite))&&((Lost_R[y-2] == 0) && (Lost_L[y-2] == LostWhite)) ) 
       {
         RoadType = Left_Ring;
         for(int x = 119;x >= y;x--)
         {
           Middle[x] =  ((119 - x)*(Middle[119] - Left[y]))/(119-y);
         }      
         break;
       }   
         
       }          
    }
   
   if(RoadType == Right_Ring)
   {
     for(int y = 119; y >= Sight;y--)
     {
       if(Right[y + 3] > Right[y + 2]  &&Right[y + 2] > Right[y + 1]  && Right[y+1] > Right[y]&&Right[y] < Right[y-1]&&Right[y -1] < Right[y-2]&&Right[y -2] < Right[y-3])
       {
         Guai_Flag = 1;
       }
       for(int x = 119;x >= y;x--)
       {
          Middle[x] =  ((119 - x)*(Right[y] - Middle[119] - 20))/(119-y);
       }
       break;
     }
   }
   
    if(RoadType == Left_Ring)
   {
     for(int y = 119; y >= Sight;y--)
     {
       if(Left[y + 3] < Left[y + 2]  && Left[y + 2] < Left[y + 1]  && Left[y+1] < Left[y]  &&Left[y] < Left[y-1] &&Left[y-1] < Left[y-2] &&Left[y-2] < Left[y-3])
       {
         Guai_Flag = 1;
       }
       for(int x = 119;x >= y;x--)
       {
          Middle[x] =  ((119 - x)*(Middle[119] - Left[y] + 20))/(119-y);
       }
       break;
     }
   }
   
   if( Guai_Flag == 1 &&(RoadType == Left_Ring || RoadType == Right_Ring))
   {
       Guai_Flag = 0;
     for(int y = 119; y >= Sight;y--)
     {
       if(Left[y + 3] < Left[y + 2]  && Left[y + 2] < Left[y + 1]  && Left[y+1] < Left[y]  &&Left[y] < Left[y-1] &&Left[y-1] < Left[y-2] &&Left[y-2] < Left[y-3])
       {
         Guai_Flag = 1;
         break;
       }
        if(Right[y + 3] > Right[y + 2]  &&Right[y + 2] > Right[y + 1]  && Right[y+1] > Right[y]&&Right[y] < Right[y-1]&&Right[y -1] < Right[y-2]&&Right[y -2] < Right[y-3])
       {
         Guai_Flag = 1;
         break;
       }
     }     
     if(Guai_Flag == 0)
     {
       RoadType ==  Normal;
     }
              
   } 
  
}
//Բ��������2
void Ring_Deal_2()
{
   int Ring_num = 0;
   static int Ring_Distance = 0;
   int Start_Ring = 0;
  
  
     for(int y=80;y>=Sight;y--)      
    {
      if(((Right[y+2] - Left[y+2]) < (Right[y] - Left[y]))&& ((Right[y] - Left[y]) < (Right[y-2] - Left[y-2]))&&((Right[y-2] - Left[y-2]) < (Right[y-4] - Left[y-4])) )
      {
         if(Right[y+2] < Right[y] && Right[y] < Right[y-2] && Right[y-2] < Right[y-4])
         {
            if(Left[y+2] > Left[y] && Left[y] > Left[y - 2]&& Left[y-2] > Left[y-4])
            {
               Ring_num++;
            }
           else
             {
             Ring_num = 0;
               }
                if(Ring_num >= 5)
                 {
                      RoadType = Ring;
                      Start_Ring = y + 5;
                      if(Ring_Distance == 0)
                      {
                        Ring_Distance = Distance;
                      }
                       break;
                  }
         }
         
      }
    
    
    }
   
  
         
    if(RoadType == Ring && Ring_Distance != 0)
    {
      for(int x = 119;x >= Sight;x--)
          {
            Middle[x] = 80 + (119 - x);
            Lost_L[x] = 0;
            Lost_R[x] = 0;
          }
    }
  if(Distance -  Ring_Distance >= 20)
  {
    RoadType = Normal;
     Ring_Distance = 0;
  }
}
//������
void OutsideCheck()
{
  
  if(CarState == Car_Start)
  {
    int BlackNum = 0;
    for(uint8 x = 0;x<CAMERA_W;x++)
    {
      if(Img[119][x] == ImgBlack)
        BlackNum++;
    }
    if(BlackNum>156)
    {
      CarState = Car_Stop;
    }
    
   
  }
  
  
}

//��ʮ�ּ�⣬ʮ�����⴦�����ö�ֵP����
void CrossCheck()
{
  uint8 Num=0;
  uint8 flag = 0;
   if(Distance_Cross == 0 || (Distance_Cross!=0 && Distance - Distance_Cross>60))
  {
    for(uint8 y = 116;y>80;y--)
    {
      if(Lost_L[y] == LostWhite && Lost_R[y] == LostWhite)
        Num++;
    }
    if(Num >29)//�ײ����߶����ߣ��Һܶ��ж�������
    {
        RoadType = Cross;
        Distance_Cross = Distance;     //��¼·����Ҫ��Ϊ�˽���ʮ�ֺ���ʮ������
        CrossNum++;
        
        if(CrossNum == 2)
        {
          Distance_Cross_GoOut = Distance;
          CrossNum = 0;
          RoadType = Normal;
        }     
    }
  }
  if(Distance_Cross_GoOut != 0)
  {
    if(Lost_L[116] ==0 && Lost_R[116] == 0)
    {
      Distance_Cross = 0;
      Distance_Cross_GoOut = 0;
      CrossNum = 0;
      RoadType = Normal;     
    }
  } 
}
/****************************
���ã���б��ʮ���жϼ�����
�������Ƿ���Ҫ�ж�
���أ��жϽ��
****************************/
int  judge_crossxieR(uint8 judge)
{
  uint8 volatile flag_guai=0,y=0,a=0;
  int Distance_YuZhi = 400;

  if(judge==0 && (RoadType == Normal || RoadType == Cross))
  {
    float xielv,xielv2,xielv3;
    for(y=110;y>=Sight;y--)
    {
      //�ұ���������ɫ��ͬʱ�������������ʧ
      if(Lost_L[y]==0 && Lost_L[y+1]==0 && Lost_R[y]==LostWhite && Lost_R[y+1]==LostWhite)
        a++;
      else
        a=0;
      //��߽翪ʼ�����ߣ��յ�֮��
      if(a>5 && Left[y]>Left[y-1] && Left[y-1]>Left[y-2] && Left[y-2]>Left[y-3] && Left[y-3]>Left[y-4] && Left[y-4]>Left[y-5])
      {
        Cross_Xielv=jisuan_xielv_orginal(Left,y,y-10);
        Cross_Xielv2=jisuan_xielv_orginal(Left,y+10,y+1);
        if(Cross_Xielv>0.05  && Cross_Xielv2<0 && Cross_Xielv2>-5 )//б�����䣬������б��ʮ�֣������������
        { 
         
          //������
          if(Lost_R[y-1]==LostWhite && Lost_R[y-2]==LostWhite)
          {                                               //�������������
            flag_guai=1;
            break;
          }
          
        }
      }
    }    
    if(flag_guai==1)
    {
      flag_guai=0;
      int j;
      int temp_x,temp_y;
      RoadType=CrossR;     
      float xielv3=10/(Left[y+10]-Left[y]);    
      for(j=y+9;j>Sight_Init;j--)
      {
        int left=(j-y-10)*Cross_Xielv2+Left[y+10];               //����б�ʲ���
        if(left>Right_Init)
          left = Right_Init;
        
        Left[j] = left;
        Middle[j]=fanjiaozheng_x(j,Left[j],ROAD_HALF);  //��У������
        Right[j]=fanjiaozheng_x(j,Left[j],ROAD_W);
        
        if(Middle[j]>Right_Init)                        //��ֹ�����ˣ�Խ����
          Middle[j]=Right_Init;
        if(Right[j]>Right_Init)
          Right[j]=Right_Init;
        Lost_L[j]=Lost_R[j]=0;
      }
      Sight_L=Sight_R=Sight_Init;
      return 1;      
    }
  }
  return 0;
}
/****************************
���ã���б��ʮ���жϼ�����
�������Ƿ���Ҫ�ж�
���أ��жϽ��
****************************/
int judge_crossxieL(uint8 judge)
{
  uint8 volatile flag_guai=0,y=0,a=0;
  int Distance_YuZhi = 400;                                                //��ʮ��3���ڱ���б��ʮ���ж�

  if(judge==0 && (RoadType == Normal || RoadType == Cross))
  {
    float xielv,xielv2;
    for(Row=108;Row>=Sight;Row--)
    {
      //���������ʧ��ͬʱ�ұ���������ʧ
      if(Lost_R[Row]==0  && Lost_R[Row+1]==0 && Lost_L[Row]==LostWhite && Lost_L[Row+1]==LostWhite)
        a++;
      else
        a=0;
      //�ұ߽翪ʼ�����ߣ��յ�֮��
      if(a>5 && Right[Row]<Right[Row-1]&& Right[Row-1]<Right[Row-2] && Right[Row-2]<Right[Row-3] && Right[Row-3]<Right[Row-4]&& Right[Row-4]<Right[Row-5])
      {
        
        
        Cross_Xielv=jisuan_xielv_orginal(Right,Row,Row-10);
        Cross_Xielv2=jisuan_xielv_orginal(Right,Row+10,Row+1);
        if(Cross_Xielv<-0.05 && Cross_Xielv2>0 && Cross_Xielv2<5)
        {
          
          //������
          if(Lost_L[Row-1]==LostWhite && Lost_L[Row-2]==LostWhite)
          {                                                //�������������
            flag_guai=1;
            break;
          }
        }
        
      }
    } 
    
    if(flag_guai==1 )//�ҵ��յ�
    {
      flag_guai=0;
      int j;
      int temp_x,temp_y;
      RoadType=CrossL;
      float xielv3=10/(Right[Row+10]-Right[Row]);
      for(j=Row+9;j>Sight_Init;j--)
      {
        int right=(j-Row-10)*Cross_Xielv2+Right[Row+10];       //��б�ʲ���

        if(right<Left_Init)
          right=Left_Init;
        
        Right[j] = right;
        Middle[j]=fanjiaozheng_x(j,Right[j],-ROAD_HALF);    //��У������
        Left[j]=fanjiaozheng_x(j,Right[j],-ROAD_W);
        
        if(Middle[j]<Left_Init)
          Middle[j]=Left_Init;
        if(Left[j]<Left_Init)
          Left[j]=Left_Init;
        
        Lost_L[j]=Lost_R[j]=0;
      }
      Sight_L=Sight_R=Sight_Init;
      return 1;
    }
  }
  return 0;
}
/****************************
���ã���������ƫ��
���أ�ͼ��ƫ��
****************************/
float jisuan_piancha()
{
  int y=0;
  int Sight_Temp;
  float piancha_avr=0;
  float piancha=0;
  ValidLine=0;         //��Ч������
  if(Sight>Sight_Init_Turn)
    Sight_Temp = Sight;
  else
    Sight_Temp = Sight_Init_Turn;
  for(y=119;y>=Sight_Temp;y--)  //�����߽���Զ�õ�1��5��������ƫ����Զ�õ�1��2
  {
    if((Lost_L[y]==LostWhite && Lost_R[y]==LostWhite) || (Lost_L[y]==LostBlack && Lost_R[y]==LostBlack))        
      {
        
      }      
      else
      {
        piancha+=(79-(int)(Middle[y])*Img_P[y/10]);
        ValidLine++;              //�ۼ���Ч����
      }
  }
  piancha_avr=(piancha)/ValidLine;    //ƽ��ƫ�����Ҫ�ĸ���Ȩ�ر�
  /*
  if(piancha_avr>54)
    piancha_avr=54;
  if(piancha_avr<-54)
    piancha_avr=-54;*/
  
  return piancha_avr;                               //+10; //�������ͷ��
}
/****************************
���ã������߼��
****************************/
void StartLineCheck()
{
    int  Stop_Num = 0;
  
 
    if(CarState == Car_Start && Distance > 200)
  {
   
    for(uint8 x = 2;x<CAMERA_W-3;x++)
    {
      if(Img[100][x+1] == ImgBlack  &&Img[100][x] == ImgBlack  && Img[100][x-1] == ImgWhite&& Img[100][x-2] == ImgWhite)
      {
        Stop_Num++;
        
      }
     if(Stop_Num >= 10)
     {
        Stop_Num = 0;
        CarState = Car_Stop;
        break;
      }
    }
  }
}















