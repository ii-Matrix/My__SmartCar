#ifndef MY_VAR_H
#define MY_VAR_H

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH2
#define MOTOR2_PWM  FTM_CH3
#define MOTOR3_PWM  FTM_CH4
#define MOTOR4_PWM  FTM_CH5
#define MOTOR_HZ    10000//(20*1000)


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH1
#define S3010_HZ    (100)


/******************摄像头及图像*****************/
typedef enum           //赛道类型
{
  Normal,                 //正常                    0
  GoOutBend,             //出弯                     1
  Cross,                 //正入十字                 2
  CrossL,                //斜入十字，向左转         3
  CrossR,                //斜入十字，向右转         4
  StartLine,             //起跑线                  5
  Obstacle_L,            //左障碍                  6
  Obstacle_R,             //右障碍                 7
  Ring,                    //圆环                   8
  Left_Ring,              //左出圆环                9
  Right_Ring,              //右出圆环             10
  Straight1,
}Road_Type;
extern Road_Type RoadType,RoadType_Last;
typedef enum //调用函数的名称
{
  NoError,          //0
  MiddleCorrect,    //1            
  MiddleCorrect2,
  LeftCorrect_1,    //2
  LeftCorrect_2,    //3
  LeftCorrect_3,    //4
  LeftCorrect_4,    //5
  LeftCorrect_5,    //6
  LeftCorrect_6,    //7
  RightCorrect_1,   //8
  RightCorrect_2,   //9
  RightCorrect_3,   //10
  RightCorrect_4,   //11
  RightCorrect_5,   //12
  RightCorrect_6,   //13
  Rod_L,
  Rod_R,
  
}Error_Type;
extern Error_Type ErrorType;
typedef enum
{
  Curve_Null,
  Straight,
  GoInBend,
  Bend,
}Curve_Type;
extern Curve_Type CurveType;
#define ImgWhite 33
#define ImgBlack 126
#define ImgGreen 2
#define ImgBlue  3
#define ImgRed   4
#define LostWhite 1
#define LostBlack 2
#define ROAD_HALF 25
#define ROAD_W    50

#define RowMax  73      //计算偏差时所用的行数总和
#define VarReceiveMax 25
#define VarSendMax    25

extern uint8 ImgBufferAgo[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10],ImgBufferNow[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10];//原始压缩图像存储缓冲区
extern uint8 Img[CAMERA_H][CAMERA_W];           //解压缩后的图像
extern uint8 real_x[CAMERA_H][CAMERA_W],real_y[CAMERA_H][CAMERA_W];//真实坐标
extern float Img_P[12];                        //图像权重比，每10行一个权重比
extern float a2,b2,c2,d2,e2,f2,v2;                //反校正系数
extern uint8 *ImgAgo,*ImgNow;                  //切换指针
extern uint8 Left[CAMERA_H],Middle[CAMERA_H],Right[CAMERA_H];//左,中，右线值
extern int Last_L,Last_R;                    //上一行的边界值
extern uint8 Jizhun_L[3],Jizhun_R[3];          //基准数组
extern uint8 Lost_L[CAMERA_H],Lost_R[CAMERA_H];        //丢线情况
extern int Left_Init,Right_Init,Middle_Init;           //左中右初始值，防止搜线时数组越界
extern int Sight_Last,Sight,Sight_L,Sight_R,Sight_Init,Sight_L_Init,Sight_R_Init;//前瞻及初始化
extern int Sight_Init_Turn;
extern uint8 Lost_L_Num,Lost_R_Num;    //丢线累计
extern uint8 ValidLine;                //有效行数
extern uint32 ImgNum_GoInBend;         //入弯图片记录
extern uint32 Img_GoOutBend;           //出弯图片记录
extern uint32 Flag_GoInBend;           //直线入弯标志位
extern uint32 Flag_GoOutBend;          //直线出弯标志位
extern uint32 ImgNum;                  //图片数目
extern uint32 ImgStopNum;                      //停止时的图片ID
extern uint32 StraightNum;             //连续直道的数目
extern uint32 BendNum;                 //连续弯道的数目
extern uint8  CrossNum;                //十字数目
extern float MiddleError,MiddleError_Last,MiddleError_Previous;              //本场图像偏差,上场，上上场
extern float AbsMiddleError;           //图像偏差的绝对值
extern float ImgCurveDegree;           //中线弯曲程度系数

/******************舵机*****************/
extern float DP_A,DP_B,DP_C;   //舵机动态P的参数
extern float DD_A;
extern float Error_Pwm;
extern int Pwm_mid;
extern int Pwm_out;
extern float  Direct_P;
extern   float  Direct_D;

/*****************速度控制*********************/
extern uint16 SpeedMin;                    //最小速度
extern uint16 SpeedGoal;                   //目标速度
extern float Motor_P;                    //电机P参数
extern float Motor_I;                    //电机I参数
extern float Speed_State;
extern int SpeedNow1;                 //当前速度
extern int Speed_Test1;
extern int CarTime;                    //车子运行一圈的时间
extern float SpeedAverage;               //一圈结束后平均速度
extern float Distance,Distance_Last;                 //路程，单位：厘米
extern float Distance_StartLine;       //起跑线路程记录
extern float Distance_GoOutBend;       //出弯路程记录
extern  float Distance_GoInBend;
extern  float Distance_Cross,Distance_Cross_GoOut;
extern long Count_Total,Count_Total_Last;               //脉冲累计值
extern int PWM_Motor1;
extern uint16  SpeedMid;
/*****************车*********************/

#define  Car_Start   1
#define  Car_Stop    0

extern uint8  CarState; 




/*****************LCD*********************/
extern int16 page,LastPage;             //页数
extern int16 Img_LCD_Loaction_X,Img_LCD_Loaction_Y;      //图像在LCD上的位置

extern void My_Run();
#endif
