#include "My_Lvbo.h"

extern int Row;
float XieLv_R,XieLv_L;
int MostLeftInRight_X;    //�ұ߽������ߵ�
int MostLeftInRight_Y ;
int MostRightInLeft_X;    //��߽�����ұ�
int MostRightInLeft_Y ;
int RodNum = 0;
int RodRemove_Y_Start_L,RodRemove_Y_Stop_L,RodRemove_Y_Start_R,RodRemove_Y_Stop_R;
/****************************
���ã�������ʱ�ϳ����ص����߷������
���أ���
****************************/
void Lvbo_Middle()
{
  int Error = 0;
  int YuZhi = 13;
  for(int y=100;y>Sight+16;y--)
  {
    Error =abs_sub(Middle[y],Middle[y-1]);
    if(Error>YuZhi)
    {//��������֮������ƫ�������ֵ�����ҷ���ͬ
      //���Һ��������
      if((Middle[y+1]>=Middle[y+2] && Middle[y+2]>=Middle[y+3] && Middle[y+3]>=Middle[y+4] && Middle[y+4]>=Middle[y+5] && Middle[y+5]>=Middle[y+6] && Middle[y+6]>=Middle[y+7] && Middle[y+7]>=Middle[y+8] && Middle[y+8]>=Middle[y+9] && Middle[y+9]>=Middle[y+10] && Middle[y+10]>=Middle[y+11] && Middle[y+11]>=Middle[y+12] && Middle[y+12]>=Middle[y+13] && Middle[y+13]>=Middle[y+14] && Middle[y+14]>=Middle[y+15] && Middle[y+15]>=Middle[y+16] && Middle[y+16]>=Middle[y+17] && Middle[y+17]>=Middle[y+18] && Middle[y+18]>=Middle[y+19] && Middle[y+1]>Middle[y+19]) 
         &&(Middle[y-1]>=Middle[y-2] && Middle[y-2]>=Middle[y-3] && Middle[y-3]>=Middle[y-4] && Middle[y-4]>=Middle[y-5] && Middle[y-5]>=Middle[y-6] && Middle[y-6]>=Middle[y-7] && Middle[y-7]>=Middle[y-8] && Middle[y-8]>=Middle[y-9] && Middle[y-9]>=Middle[y-10] && Middle[y-10]>=Middle[y-11] && Middle[y-11]>=Middle[y-12] && Middle[y-12]>=Middle[y-13] && Middle[y-13]>=Middle[y-14] && Middle[y-14]>=Middle[y-15] && Middle[y-15]>=Middle[y-16] && Middle[y-1]>Middle[y-16]))
      {
        for(int i = y;(i<=y+25) && i<110;i++)
        {
          if(Middle[i]>Middle[y-1])
            Middle[i]=Middle[y-1];     //����һ�����ŵ�ֱ��
        }
      }
      //������ҵ�����
      if((Middle[y+1]<=Middle[y+2] && Middle[y+2]<=Middle[y+3] && Middle[y+3]<=Middle[y+4] && Middle[y+4]<=Middle[y+5] && Middle[y+5]<=Middle[y+6] && Middle[y+6]<=Middle[y+7] && Middle[y+7]<=Middle[y+8] && Middle[y+8]<=Middle[y+9] && Middle[y+9]<=Middle[y+10] && Middle[y+10]<=Middle[y+11] && Middle[y+11]<=Middle[y+12] && Middle[y+12]<=Middle[y+13] && Middle[y+13]<=Middle[y+14] && Middle[y+14]<=Middle[y+15] && Middle[y+15]<=Middle[y+16] && Middle[y+16]<=Middle[y+17] && Middle[y+17]<=Middle[y+18] && Middle[y+18]<=Middle[y+19] && Middle[y+1]<Middle[y+19]) 
         &&(Middle[y-1]<=Middle[y-2] && Middle[y-2]<=Middle[y-3] && Middle[y-3]<=Middle[y-4] && Middle[y-4]<=Middle[y-5] && Middle[y-5]<=Middle[y-6] && Middle[y-6]<=Middle[y-7] && Middle[y-7]<=Middle[y-8] && Middle[y-8]<=Middle[y-9] && Middle[y-9]<=Middle[y-10] && Middle[y-10]<=Middle[y-11] && Middle[y-11]<=Middle[y-12] && Middle[y-12]<=Middle[y-13] && Middle[y-13]<=Middle[y-14] && Middle[y-14]<=Middle[y-15] && Middle[y-15]<=Middle[y-16] && Middle[y-1]<Middle[y-16]))
      {
        for(int i = y;(i<=y+25) && i<110;i++)
        {
          if(Middle[i]<Middle[y-1])
            Middle[i]=Middle[y-1];
        }
      }
    }
  }
}
/****************************
���ã�������ֱͼ���Ż�������ת�������
���أ���
****************************/
void Img_GoOutBend_Optimize()
{
  if(Distance_GoOutBend == 0 && RoadType == Normal)
  {
    
    if(Sight-Sight_Init_Turn<=6)//���ıȽ�Զ
    {
      //�����
      int YuZhi = 15; 
      if((Lost_L_Num>YuZhi) && (Lost_R_Num<YuZhi) && Right[Sight]<80 && Left[119]<15)//��߶��߽϶�,�ұ߶�����
      {                                                                 //�ʹ�S������
        uint8 Num = 0;
        for(uint8 y = Sight_Init_Turn;y<(Sight_Init_Turn+13);y++)
        {
          if(Middle[y] == Left_Init)
            Num++;
        }
        if(Num>9) // Զ�����߶��ǲ���ͷ��
        {//������������
          RoadType = GoOutBend;   //�����������޸�Ϊ����
          Distance_GoOutBend = Distance;
          for(uint8 y =119;y>Sight_Init_Turn;y--)
          {
            Middle[y] = (Left[y]+Right[y])/2;
          }
        }
      }
      if((Lost_R_Num>YuZhi) && (Lost_L_Num<YuZhi) && Left[Sight]>80 && Right[119]>144)//��߶��߽���,�ұ߶��߶�
      {                                                               //�ʹ�S������              
        uint8 Num = 0;
        for(uint8 y = Sight_Init_Turn;y<(Sight_Init_Turn+13);y++)
        {
          if(Middle[y] == Right_Init)
            Num++;
        }
        if(Num>9) // Զ�����߶��ǲ���ͷ��
        {//������������
          RoadType = GoOutBend;   //�����������޸�Ϊ����
          Distance_GoOutBend = Distance;
          for(uint8 y =119;y>Sight_Init_Turn;y--)
          {
            Middle[y] = (Left[y]+Right[y])/2;
          }
        }
      }
    }
  }
  int YuZhi = 11;                //��֤�����Ż���������������12cm��·��
  if(Distance_GoOutBend != 0 && Distance-Distance_GoOutBend<YuZhi)
  {
    RoadType = GoOutBend;   //�����������޸�Ϊ����
    for(uint8 y =119;y>Sight_Init_Turn;y--)
    {
      Middle[y] = (Left[y]+Right[y])/2;
    }    
  }
  if(Distance_GoOutBend != 0 && Distance-Distance_GoOutBend>=YuZhi)
  {
    RoadType = 0;
    Distance_GoOutBend = 0;
  }
}

int Lvbo_Middle2(uint8 y,int middle)
{
  int YuZhi = 10;
  uint8 Flag = 0;
  if((0==Lost_L[y-1]) && (0!=Lost_R[y-1]))     //����У��ұ�û��
  { 
    Flag =1;
    Middle[y-1]=Middle[y]+(Left[y-1]-Left[y]);  //������߱仯���Ʋ���
  }
  
  else if((0!=Lost_L[y-1]) && (0==Lost_R[y-1]))     //���û�У��ұ���
  {  
    Flag=1;
    Middle[y-1]=Middle[y]-(Right[y]-Right[y-1]);  //�����ұ߱仯���Ʋ���  
  }
  if(Flag == 1 && Middle[y]<Middle[y+1] && Middle[y]<Middle[y-1] && abs_sub(Middle[y],Middle[y+1])>YuZhi)
  {
    if(Lost_L[y] == LostWhite && Lost_R[y] == LostWhite)
    {
      if(Lost_R[y-1] == 0 && Lost_L[y-1] !=0)
      {
        middle=(Middle[y+3]+Middle[y+2]+Middle[y+1])/3;
        return middle;
      }
    }
  }
  if(Flag == 1 && Middle[y]>Middle[y+1] && Middle[y]>Middle[y-1] && abs_sub(Middle[y],Middle[y+1])>YuZhi)
  {
    if(Lost_L[y] == LostWhite && Lost_R[y] == LostWhite)
    {
      if(Lost_L[y-1] == 0 && Lost_R[y-1] !=0)
      {
        middle=(Middle[y+3]+Middle[y+2]+Middle[y+1])/3;
        return middle;
      }
    }  
  } 
  return middle;
}
void Lvbo_Left(uint8 y)
{
  uint8 YuZhi=20;
  if(abs_sub(Left[y],Left[y+1])>YuZhi)
  {
    Last_L=Left[y]=Left[y+1];
    if(Img[y][Left[y]]==ImgWhite)
    {
      Lost_L[y] = LostWhite;
    }
    if(Img[y][Left[y]]==ImgBlack)
    {
      Lost_L[y] = LostBlack;
    }    
  }
}
void Lvbo_Right(uint8 y)
{
  uint8 YuZhi=20;
  if(abs_sub(Right[y],Right[y+1])>YuZhi)
  {
    Last_R=Right[y]=Right[y+1];
    if(Img[y][Right[y]]==ImgWhite)
    {
      Lost_R[y] = LostWhite;
    }
    if(Img[y][Right[y]]==ImgBlack)
    {
      Lost_R[y] = LostBlack;
    }    
  }
}