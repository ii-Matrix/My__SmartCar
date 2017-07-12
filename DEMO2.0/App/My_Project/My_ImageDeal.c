#include "My_ImageDeal.h"

uint8 ImgBufferAgo[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10],ImgBufferNow[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10];//原始压缩图像存储缓冲区
uint8 Img[CAMERA_H][CAMERA_W];           //解压缩后的图像                      1
uint8 *ImgNow,*ImgAgo;                //                                 1


float Img_P[12] = {1,1,1,1,1,1,1,1,1,1,1,1};                        //图像权重比，每10行一个权重比      
float a2 = 0,b2 = 0,c2 = 0,d2 = 0,e2 = 0,f2 = 0,v2 = 0;                //反校正系数                      
uint8  Left[CAMERA_H],Middle[CAMERA_H],Right[CAMERA_H];//左,中，右线值       1
int Last_L,Last_R;                    //上一行的边界值                     1
uint8 Jizhun_L[3],Jizhun_R[3];          //基准数组                         1
uint8 Lost_L[CAMERA_H],Lost_R[CAMERA_H];        //丢线情况                   1

int Left_Init=5,Right_Init=155,Middle_Init=80;           //左中右初始值，防止搜线时数组越界               1
int Sight_Last,Sight=24,Sight_L=24,Sight_R=24,Sight_Init=24,Sight_L_Init=24,Sight_R_Init=24;//前瞻及初始化   1
int Sight_Init_Turn  = 26;
Road_Type RoadType,RoadType_Last;                 //赛道类型       2
uint8 Lost_L_Num,Lost_R_Num;    //丢线累计
uint8 ValidLine;                //有效行数



uint32 Flag_GoInBend;           //直线入弯标志位
uint32 Flag_GoOutBend;          //直线出弯标志位

uint32 ImgNum;                  //图片数目
uint32 ImgStopNum;              //停止时的图片ID
uint32 StraightNum;             //连续直道的数目
uint32 BendNum;                 //连续弯道的数目
uint32 Img_GoOutBend;            //出弯时的图片数目
uint8  CrossNum;                //十字数目




float MiddleError,MiddleError_Last,MiddleError_Previous;              //本场图像偏差,上场，上上场
float AbsMiddleError;           //图像偏差的绝对值
float ImgCurveDegree;           //中线弯曲程度系数


Curve_Type CurveType;           //弯曲程度，主要为了速度控制服务
int Row;
float Cross_Xielv,Cross_Xielv2;


/****************************
作用：图像底部寻找，确定基准值
****************************/
void BottomFind()
{   
  int x=0,y=0;                              //y为纵坐标，x为横坐标
  int BlackNum_L=0,BlackNum_R=0;            //底部三行左右黑点总数
  int Start_L = 80;
  int Start_R = 80 ;                        //底部搜索起始点

 
    for(y=119;y>116;y--)                  //统计左右黑点数目，若左>右，从稍靠右开始搜索
    {
      for(int x = 79;x>4;x-=4)            //-4是为了加快速度，大致判断没必要做精确
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
      //找右基准
      for(x=Start_R;x<Right_Init;x++)
      {   //寻找跳变点
        if(Img[y][x+4]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x]==ImgWhite && Img[y][x-2]==ImgWhite && Img[y][x-4]==ImgWhite && Img[y][x-6]==ImgWhite)
        {
          Right[y]=x-1;             //记录
          Lost_R[y]=0;            //丢线标志位清零
          break;
        }
      }
      
      //找左基准
      for(x=Start_L;x>Left_Init;x--)
      {
        //寻找跳变点
        if(Img[y][x-2]==ImgBlack && Img[y][x-1]==ImgBlack && Img[y][x]==ImgWhite && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite && Img[y][x+3]==ImgWhite)
        {
          Left[y]=x+1;             //记录
          Lost_L[y]=0;            //丢线标志位清零
          break;
        }
      }
      
      Jizhun_R[119-y] = Right[y];
      Jizhun_L[119-y] = Left[y];
      Middle[y]=(Right[y]+Left[y])/2;
      //   lvbo_Jizhun(y);
    }                       
    //计算基准均值
    Last_L=(Jizhun_L[0]+Jizhun_L[1]+Jizhun_L[2])/3;
    Last_R=(Jizhun_R[0]+Jizhun_R[1]+Jizhun_R[2])/3;
}


/****************************
作用：找到边界线
****************************/
void GenZong()
{
  int x=0,y=0;
  int Window = 10;
  
  
    uint8 Final_L=Left_Init,Final_R=Right_Init,Start_L,Start_R;
    
    //一路跟踪到有效前瞻，嘎嘎
    for(y=116;y>=Sight_Init;y--)// && Sight_L==Sight_L_Init && Sight_R==Sight_R_Init;y--)//判断项这么长是因为有时候要改变Sight之类的值，结束搜索
    {
        //左边界从右向左搜 
        if(Last_L - Window > Left_Init)
          Final_L = Last_L - Window;     
       
        if(Last_L + Window < Right_Init)
          Start_L = Last_L + Window;
        else
          Start_L = Right_Init;                  //右边界
        
         Left[y]=Left_Init;                        //赋初值，判断是否未找到边界
        
        for(x=Start_L;x>Final_L;x--)     
        {
          if(Img[y][x]==ImgBlack)                        //找到黑点
          {                                            //进一步确认
            if(Img[y][x-1]==ImgBlack && Img[y][x-2]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite)
            { 
              Left[y] = x + 2;
              Lost_L[y]=0;                     //丢线标志位清零
              break;                          //退出循环 
            }
          }           
        }
        
        Last_L=Left[y];                              //保存上一次的值

        if(Left[y]==Left_Init)                    //说明没有提取到边界
        {
          Last_L=Left[y]=Refind_L(y);                //再次寻找
        }
        
        //右边界从左向右搜 

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
          if(Img[y][x]==ImgBlack)//找到黑点
          {//进一步确认
            if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite)
            { 
              Right[y]=x-2;
              Lost_R[y]=0;
              break;//退出循环
            }
          }           
        }
        Last_R=Right[y];     //保存上一次的值
        if(Right[y]==Right_Init)//说明右边界没有提取到
        {
          Last_R=Right[y]=Refind_R(y);    
        }     
    }
    
    
}
/****************************
作用：计算中线
****************************/
void jisuan_Middle()
{
  int temp_x=0,temp_y=0,temp_M=0;
  uint8 flag=0; 
  int middle=79;
  
  for(int y=116;y>=10;y--)
  {
     
      if(Lost_L[y]==0 && Lost_R[y]==0)      //两边都能找到
      {
        middle=(Left[y]+Right[y])/2;  
       }
    else if((0!=Lost_L[y]) && (0!=Lost_R[y]))     //两边都没有
      {
        if((Lost_L[y]==LostWhite) && (Lost_R[y]==LostWhite))     //两边都没有,并且丢白线，可能是十字
        {
         // middle=(Middle[y+1]+Middle[y+2]+Middle[y+3])/3;       //中点等于前三行均值,计算偏差时本行不参与

          
            Last_L=Left[y]=fanjiaozheng_x(y,middle,-ROAD_HALF);     //平移赛道宽度一半补边界线
            Last_R=Right[y]=fanjiaozheng_x(y,middle,+ROAD_HALF);   //不懂
             middle=(Left[y]+Right[y])/2;    
           
        }
        else
          middle=(Middle[y+3]+Middle[y+2]+Middle[y+1])/3;
      }
   else if((0==Lost_L[y]) && (0!=Lost_R[y]))     //左边有，右边没有
      { 
        Lost_R_Num++;
        if(Lost_R[y+1] ==LostWhite && Lost_L[y+1] == LostWhite)
          middle = Middle[y+1];
        else       
          
          middle=Middle[y+1]+(Left[y]-Left[y+1]);//+(int)(Lost_R_Num*Lost_R_Num*ImgCurveDegree);  //按照左边变化趋势补线
      }
      
      else if((0!=Lost_L[y]) && (0==Lost_R[y]))     //左边没有，右边有
      { 
        Lost_L_Num++;
        if(Lost_R[y+1] ==LostWhite && Lost_L[y+1] == LostWhite)
          middle = Middle[y+1];
        else
          middle=Middle[y+1]-(Right[y + 1]-Right[y]);//-(int)(Lost_L_Num*Lost_L_Num*ImgCurveDegree);  //按照右边变化趋势补线   
      }
   

    if(middle>Right_Init)
      middle=Right_Init;
    if(middle<Left_Init)
      middle=Left_Init;
    Middle[y]=middle;
  }
  //  Lvbo_Middle();//缝合
  
  
}
/****************************
作用：未找到边界线时再次寻找
参数：当前行的纵坐标
返回：找到的边界线横坐标
****************************/
uint8 Refind_L(uint8 y)//再找一次左边界
{
  int Final,Start,x;            //。。。又吃了一次数据类型的亏，不能是uint8，负数变正后可能比159大
  
   
    uint8 Final_L=Left_Init,Final_R=Right_Init,Start_L,Start_R;
    int window = 22;
    //左边界从右向左搜 
    if((int)Left[y+1]-window>Left_Init)
      Final_L=Left[y+1]-window;       
    if(Left[y+1]+window<Right_Init)
      Start_L=Left[y+1]+window;
    else
      Start_L=Right_Init;
    
    for(x=Start_L;x>Final_L;x--)     
    {
      if(Img[y][x]==ImgBlack)//找到黑点
      {//进一步确认
        if(Img[y][x-1]==ImgBlack && Img[y][x-1]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite)
        { 
          Lost_L[y]=0;        //丢线标志位清零
          return x+2;  
        }
      } 
    }          
    
    //依旧找不到，启动中心扩散搜索
    if(Left[y+1]<78 && Right[y+1]>82 )      //中心扩散搜索，多用于正入十字
    {
      for(x=80;x>Left_Init;x--)     
      {
        if(Img[y][x]==ImgBlack)//找到黑点
        {//进一步确认
          if(Img[y][x-1]==ImgBlack && Img[y][x-2]==ImgBlack && Img[y][x+1]==ImgWhite && Img[y][x+2]==ImgWhite && Img[y][x+3]==ImgWhite)
          { 
            Lost_L[y]=0;        //丢线标志位清零
            return x+2;            
          }           
        }
      }
    }
    
    
    if(Img[y][Left[y+1]]==ImgWhite )//本行是白色                          
      Lost_L[y]=LostWhite;
    
    if(Img[y][Left[y+1]]==ImgBlack)//本行是黑色
      Lost_L[y]=LostBlack;
    
    return Left[y+1];//还是没找到，返回上一行的值
  

}
/****************************
作用：未找到边界线时再次寻找
参数：当前行的纵坐标
返回：找到的边界线横坐标
****************************/
uint8 Refind_R(uint8 y)         //再找一次右边界
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
    //右边界从左向右搜 
    for(x=Start_R;x<Final_R;x++) 
    {
      if(Img[y][x]==ImgBlack)//找到黑点
      {//进一步确认
        if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite)
        { 
          Lost_R[y]=0;        //丢线标志位清零
          return x-2; 
        }
      }           
    }
    
    if(Left[y+1]<78 && Right[y+1]>82 )  
      //依旧找不到，启动中心扩散搜索
    {
      for(int x=80;x<Right_Init;x++)
      {
        if(Img[y][x]==ImgBlack)//找到黑点
        {//进一步确认
          if(Img[y][x+1]==ImgBlack && Img[y][x+2]==ImgBlack && Img[y][x-1]==ImgWhite && Img[y][x-2]==ImgWhite && Img[y][x-3]==ImgWhite)
          { 
            Lost_R[y]=0;        //丢线标志位清零
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
作用：动态前瞻的确定
****************************/
void ForeSight()
{
  
    for(int y=80;y>=Sight_Init;y--)      
    { 
      //纵向连续三点丢失，且都是黑色，则判断是赛道外
      // if(Img[y][Left[y]] == ImgBlack && Img[y-1][Left[y-1]] == ImgBlack && Img[y-2][Left[y-2]] == ImgBlack)
      if(Lost_L[y] == LostBlack && Lost_L[y-1] == LostBlack && Lost_L[y-2] == LostBlack)
      {
        Sight_L=y+1;
        break;
      }
      //左边界线比较靠右
      if(y<80 && Left[y]>150 && Left[y+1]>140 && Left[y]>Left[y+1])   
      {
        Sight_L=y;
        break;
      }
    }
  
  
  
    for(int y=80;y>=Sight_Init;y--)
    {   //纵向连续三点丢失，且都是黑色，则判断是赛道外
      //   if(Img[y][Right[y]] == ImgBlack && Img[y-1][Right[y-1]] == ImgBlack && Img[y-2][Right[y-2]] == ImgBlack)
      if(Lost_R[y] == LostBlack && Lost_R[y-1] == LostBlack && Lost_R[y-2] == LostBlack)
      { 
        Sight_R=y+1;
        break;    
      }
      //      右边界线比较靠左
      if(y<80 && Right[y]<10 && Right[y+1]<20 && Right[y]<Right[y+1])   
      {
        Sight_R=y;
        break;
      }    
    
  }

  if(Sight_L>Sight_R)   // 前瞻取最远的
    Sight=Sight_R;
  else
    Sight=Sight_L;
  
}
/****************************
作用：判断赛道类型
****************************/
void judge_road()
{
  uint8 judge=0;        //引入这个变量的目的主要是判断出一个赛道类型后不再进行其他赛道类型的判断，减少误判
 
  //和超车区冲突，要改！！
 // CrossCheck();                      //入十字检测
 // judge = judge_crossxieL(judge);    //左斜出十字          
 // judge = judge_crossxieR(judge);    //右斜出十字
  
    //Ring_Deal();
     Ring_Deal_2();
  StraightCheck();                  //直线检测
  //GoInBendCheck();                  //入弯检测
  OutsideCheck();                   //出界检测
  
}

/****************************
未验证
作用：直道判断
****************************/
void StraightCheck()
{
  {   
    uint8 Yuzhi=10;
    if(AbsMiddleError<Yuzhi && Sight - Sight_Init<2)//偏差小，前瞻远
    {     
        RoadType = Straight1;
        if(Count_Total-Count_Total_Last>335)//每5CM更新一次
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
      if(StraightNum>0)                //至少连续两次判断为直道，才将赛道类型赋值为直道
    {                                //毕竟判断为直道后，电机速度会瞬加，这样做更保险
       RoadType = Straight1;
    }
  }
}
/******************
******圆环的处理
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
//圆环处理函数2
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
//出界检测
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

//入十字检测，十字特殊处理，采用定值P方案
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
    if(Num >29)//底部两边都丢线，且很多行都是这样
    {
        RoadType = Cross;
        Distance_Cross = Distance;     //记录路程主要是为了将出十字和入十字区别开
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
作用：右斜出十字判断及补线
参数：是否需要判断
返回：判断结果
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
      //右边连续丢白色的同时，左边连续不丢失
      if(Lost_L[y]==0 && Lost_L[y+1]==0 && Lost_R[y]==LostWhite && Lost_R[y+1]==LostWhite)
        a++;
      else
        a=0;
      //左边界开始向左走，拐点之后
      if(a>5 && Left[y]>Left[y-1] && Left[y-1]>Left[y-2] && Left[y-2]>Left[y-3] && Left[y-3]>Left[y-4] && Left[y-4]>Left[y-5])
      {
        Cross_Xielv=jisuan_xielv_orginal(Left,y,y-10);
        Cross_Xielv2=jisuan_xielv_orginal(Left,y+10,y+1);
        if(Cross_Xielv>0.05  && Cross_Xielv2<0 && Cross_Xielv2>-5 )//斜率跳变，可能是斜入十字，可能是向左拐
        { 
         
          //防误判
          if(Lost_R[y-1]==LostWhite && Lost_R[y-2]==LostWhite)
          {                                               //和正常弯道区别开
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
        int left=(j-y-10)*Cross_Xielv2+Left[y+10];               //按照斜率补线
        if(left>Right_Init)
          left = Right_Init;
        
        Left[j] = left;
        Middle[j]=fanjiaozheng_x(j,Left[j],ROAD_HALF);  //反校正补线
        Right[j]=fanjiaozheng_x(j,Left[j],ROAD_W);
        
        if(Middle[j]>Right_Init)                        //防止补过了，越界了
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
作用：左斜出十字判断及补线
参数：是否需要判断
返回：判断结果
****************************/
int judge_crossxieL(uint8 judge)
{
  uint8 volatile flag_guai=0,y=0,a=0;
  int Distance_YuZhi = 400;                                                //入十字3米内保持斜出十字判断

  if(judge==0 && (RoadType == Normal || RoadType == Cross))
  {
    float xielv,xielv2;
    for(Row=108;Row>=Sight;Row--)
    {
      //左边连续丢失的同时右边连续不丢失
      if(Lost_R[Row]==0  && Lost_R[Row+1]==0 && Lost_L[Row]==LostWhite && Lost_L[Row+1]==LostWhite)
        a++;
      else
        a=0;
      //右边界开始向右走，拐点之后
      if(a>5 && Right[Row]<Right[Row-1]&& Right[Row-1]<Right[Row-2] && Right[Row-2]<Right[Row-3] && Right[Row-3]<Right[Row-4]&& Right[Row-4]<Right[Row-5])
      {
        
        
        Cross_Xielv=jisuan_xielv_orginal(Right,Row,Row-10);
        Cross_Xielv2=jisuan_xielv_orginal(Right,Row+10,Row+1);
        if(Cross_Xielv<-0.05 && Cross_Xielv2>0 && Cross_Xielv2<5)
        {
          
          //防误判
          if(Lost_L[Row-1]==LostWhite && Lost_L[Row-2]==LostWhite)
          {                                                //和正常弯道区别开
            flag_guai=1;
            break;
          }
        }
        
      }
    } 
    
    if(flag_guai==1 )//找到拐点
    {
      flag_guai=0;
      int j;
      int temp_x,temp_y;
      RoadType=CrossL;
      float xielv3=10/(Right[Row+10]-Right[Row]);
      for(j=Row+9;j>Sight_Init;j--)
      {
        int right=(j-Row-10)*Cross_Xielv2+Right[Row+10];       //按斜率补线

        if(right<Left_Init)
          right=Left_Init;
        
        Right[j] = right;
        Middle[j]=fanjiaozheng_x(j,Right[j],-ROAD_HALF);    //反校正补线
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
作用：计算中线偏差
返回：图像偏差
****************************/
float jisuan_piancha()
{
  int y=0;
  int Sight_Temp;
  float piancha_avr=0;
  float piancha=0;
  ValidLine=0;         //有效行清零
  if(Sight>Sight_Init_Turn)
    Sight_Temp = Sight;
  else
    Sight_Temp = Sight_Init_Turn;
  for(y=119;y>=Sight_Temp;y--)  //搜索边界最远用到1米5，但计算偏差最远用到1米2
  {
    if((Lost_L[y]==LostWhite && Lost_R[y]==LostWhite) || (Lost_L[y]==LostBlack && Lost_R[y]==LostBlack))        
      {
        
      }      
      else
      {
        piancha+=(79-(int)(Middle[y])*Img_P[y/10]);
        ValidLine++;              //累计有效行数
      }
  }
  piancha_avr=(piancha)/ValidLine;    //平均偏差，后期要改各行权重比
  /*
  if(piancha_avr>54)
    piancha_avr=54;
  if(piancha_avr<-54)
    piancha_avr=-54;*/
  
  return piancha_avr;                               //+10; //解决摄像头歪
}
/****************************
作用：起跑线检测
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















