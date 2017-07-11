#include "My_Math.h"

float jisuan_xielv(uint8 line[60],uint8 xielvstart,uint8 xielvstop)//��С���˷�
{
  float avr_x=0,avr_y=0,K=0;
  uint32 sum_x=0,sum_y=0,sum_xy=0,sum_xx=0,sum_yy=0;;//uint16̫С
  float n=0;//ѡȡ��ĸ�����������float�ͣ�����18/10=1������Դ�
  int i=0;//int�ͷ�����watch������鿴
  uint16 temp_x=0,temp_y=0;
  
  n=(xielvstart-xielvstop);
  for(temp_x=0,temp_y=0,i=xielvstart;i>xielvstart-n;i--)
  {
    
    temp_y=real_x[i][line[i]];  //y��x��x��y������ֱ�ߵ�б�ʷ�ĸ���㣬�����б���������
    temp_x=148-real_y[i][line[i]];
    sum_x+=temp_x;
    sum_y+=temp_y;
    sum_xx+=(temp_x*temp_x);
    sum_yy+=(temp_y*temp_y);
    sum_xy+=(temp_x*temp_y);   
  }
  avr_x=sum_x/n;
  avr_y=sum_y/n;
  //  K=(sum_xx-n*avr_x*avr_x)/(sum_xy-n*avr_x*avr_y);
  //   K=(sum_xy-n*avr_x*avr_y)/(sum_yy-n*avr_y*avr_y);
  K=(sum_xy-n*avr_x*avr_y)/(sum_xx-n*avr_x*avr_x);
  // printf("����б��:%f\n",K);
  return K;
  
}
/*************************************************
���ã���С���˼���ԭͼ����л�����ĳһ���ֵ�б��
������
���أ���ֵ
**************************************************/
float jisuan_xielv_orginal(uint8 line[60],uint8 xielvstart,uint8 xielvstop)//��С���˷�
{
  float avr_x=0,avr_y=0,K=0;
  uint32 sum_x=0,sum_y=0,sum_xy=0,sum_xx=0,sum_yy=0;;//uint16̫С
  float n=0;//ѡȡ��ĸ�����������float�ͣ�����18/10=1������Դ�
  int i=0;//int�ͷ�����watch������鿴
  uint16 temp_x=0,temp_y=0;
  
  n=(xielvstart-xielvstop);
  for(temp_x=0,temp_y=0,i=xielvstart;i>xielvstart-n;i--)
  {
    
    temp_y=line[i];  //y��x��x��y������ֱ�ߵ�б�ʷ�ĸ���㣬�����б���������
    temp_x=i;
    sum_x+=temp_x;
    sum_y+=temp_y;
    sum_xx+=(temp_x*temp_x);
    sum_yy+=(temp_y*temp_y);
    sum_xy+=(temp_x*temp_y);   
  }
  avr_x=sum_x/n;
  avr_y=sum_y/n;
  //  K=(sum_xx-n*avr_x*avr_x)/(sum_xy-n*avr_x*avr_y);
  //   K=(sum_xy-n*avr_x*avr_y)/(sum_yy-n*avr_y*avr_y);
  K=(sum_xy-n*avr_x*avr_y)/(sum_xx-n*avr_x*avr_x);
  // printf("����б��:%f\n",K);
  return K; 
}
/*****************************
���ã����㷨�������ߵ�����
����������ĺ�������
���أ�����
*****************************/
float jisuan_qulv(uint8 temp_x1,uint8 temp_y1,uint8 temp_x2,uint8 temp_y2,uint8 temp_x3,uint8 temp_y3)
{
  float AB,BC,AC,K,S;
  uint8 x1=real_x[temp_y1][temp_x1];
  uint8 y1=real_y[temp_y1][temp_x1];
  uint8 x2=real_x[temp_y2][temp_x2];
  uint8 y2=real_y[temp_y2][temp_x2];
  uint8 x3=real_x[temp_y3][temp_x3];
  uint8 y3=real_y[temp_y3][temp_x3];
  S=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2.0;
  AB=mysqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
  BC=mysqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
  AC=mysqrt((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1));
  K=4*S/(AB*BC*AC);  
  
  return K;
  
}
/*****************************
���ã��������ľ���ֵ
������������
���أ�����ֵ
*****************************/
float Myabsfloat(float a)
{
  if(a<0)
    return -a;
  else
    return a;
}
/*****************************
���ã��������������
������������ĺ�������
���أ����
*****************************/
int jisuan_area(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3)
{
  
  int S=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
  if(S<0)
    S=-S;
  return S;
}
/*****************************
���ã������޷���������ľ���ֵ
����������������޷�����
���أ���ֵ
*****************************/
uint32 abs_sub(uint32 diff1,uint32 diff2)
{
  uint32 temp;
  if(diff1>diff2)
  {
    temp=diff1-diff2;
  }
  else
  {
    temp=diff2-diff1;
  }
  return temp;
}