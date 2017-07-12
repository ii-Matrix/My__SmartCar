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


/******************����ͷ��ͼ��*****************/
typedef enum           //��������
{
  Normal,                 //����                    0
  GoOutBend,             //����                     1
  Cross,                 //����ʮ��                 2
  CrossL,                //б��ʮ�֣�����ת         3
  CrossR,                //б��ʮ�֣�����ת         4
  StartLine,             //������                  5
  Obstacle_L,            //���ϰ�                  6
  Obstacle_R,             //���ϰ�                 7
  Ring,                    //Բ��                   8
  Left_Ring,              //���Բ��                9
  Right_Ring,              //�ҳ�Բ��             10
  Straight1,
}Road_Type;
extern Road_Type RoadType,RoadType_Last;
typedef enum //���ú���������
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

#define RowMax  73      //����ƫ��ʱ���õ������ܺ�
#define VarReceiveMax 25
#define VarSendMax    25

extern uint8 ImgBufferAgo[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10],ImgBufferNow[CAMERA_SIZE+360+(VarReceiveMax+VarSendMax)*3+10];//ԭʼѹ��ͼ��洢������
extern uint8 Img[CAMERA_H][CAMERA_W];           //��ѹ�����ͼ��
extern uint8 real_x[CAMERA_H][CAMERA_W],real_y[CAMERA_H][CAMERA_W];//��ʵ����
extern float Img_P[12];                        //ͼ��Ȩ�رȣ�ÿ10��һ��Ȩ�ر�
extern float a2,b2,c2,d2,e2,f2,v2;                //��У��ϵ��
extern uint8 *ImgAgo,*ImgNow;                  //�л�ָ��
extern uint8 Left[CAMERA_H],Middle[CAMERA_H],Right[CAMERA_H];//��,�У�����ֵ
extern int Last_L,Last_R;                    //��һ�еı߽�ֵ
extern uint8 Jizhun_L[3],Jizhun_R[3];          //��׼����
extern uint8 Lost_L[CAMERA_H],Lost_R[CAMERA_H];        //�������
extern int Left_Init,Right_Init,Middle_Init;           //�����ҳ�ʼֵ����ֹ����ʱ����Խ��
extern int Sight_Last,Sight,Sight_L,Sight_R,Sight_Init,Sight_L_Init,Sight_R_Init;//ǰհ����ʼ��
extern int Sight_Init_Turn;
extern uint8 Lost_L_Num,Lost_R_Num;    //�����ۼ�
extern uint8 ValidLine;                //��Ч����
extern uint32 ImgNum_GoInBend;         //����ͼƬ��¼
extern uint32 Img_GoOutBend;           //����ͼƬ��¼
extern uint32 Flag_GoInBend;           //ֱ�������־λ
extern uint32 Flag_GoOutBend;          //ֱ�߳����־λ
extern uint32 ImgNum;                  //ͼƬ��Ŀ
extern uint32 ImgStopNum;                      //ֹͣʱ��ͼƬID
extern uint32 StraightNum;             //����ֱ������Ŀ
extern uint32 BendNum;                 //�����������Ŀ
extern uint8  CrossNum;                //ʮ����Ŀ
extern float MiddleError,MiddleError_Last,MiddleError_Previous;              //����ͼ��ƫ��,�ϳ������ϳ�
extern float AbsMiddleError;           //ͼ��ƫ��ľ���ֵ
extern float ImgCurveDegree;           //���������̶�ϵ��

/******************���*****************/
extern float DP_A,DP_B,DP_C;   //�����̬P�Ĳ���
extern float DD_A;
extern float Error_Pwm;
extern int Pwm_mid;
extern int Pwm_out;
extern float  Direct_P;
extern   float  Direct_D;

/*****************�ٶȿ���*********************/
extern uint16 SpeedMin;                    //��С�ٶ�
extern uint16 SpeedGoal;                   //Ŀ���ٶ�
extern float Motor_P;                    //���P����
extern float Motor_I;                    //���I����
extern float Speed_State;
extern int SpeedNow1;                 //��ǰ�ٶ�
extern int Speed_Test1;
extern int CarTime;                    //��������һȦ��ʱ��
extern float SpeedAverage;               //һȦ������ƽ���ٶ�
extern float Distance,Distance_Last;                 //·�̣���λ������
extern float Distance_StartLine;       //������·�̼�¼
extern float Distance_GoOutBend;       //����·�̼�¼
extern  float Distance_GoInBend;
extern  float Distance_Cross,Distance_Cross_GoOut;
extern long Count_Total,Count_Total_Last;               //�����ۼ�ֵ
extern int PWM_Motor1;
extern uint16  SpeedMid;
/*****************��*********************/

#define  Car_Start   1
#define  Car_Stop    0

extern uint8  CarState; 




/*****************LCD*********************/
extern int16 page,LastPage;             //ҳ��
extern int16 Img_LCD_Loaction_X,Img_LCD_Loaction_Y;      //ͼ����LCD�ϵ�λ��

extern void My_Run();
#endif
