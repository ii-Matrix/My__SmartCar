/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"



/*
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
 */
void  main(void)
{
    Init_All(); 
    while(1)
    {
     
     
        camera_get_img();                                   //摄像头获取图像
       
        //黑白摄像头
     //   LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
       // vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
}




void My_Run()
{
   img_extract((uint8*)Img,ImgAgo,CAMERA_SIZE);    //解压缩,一维数组变二维数组
   StartLineCheck();                               //起跑线检测
  
   BottomFind();                                   //底部寻找
   GenZong();                                      //跟踪边缘  

   ForeSight();                                    //确定前瞻

   jisuan_Middle();                                //计算中线

  judge_road();                                    //判断赛道类型         
  MiddleError = jisuan_piancha();                   //计算偏差
  AbsMiddleError = Myabsfloat(MiddleError); 
  

  Direct_PD();
  SpeedSet();                                     //更改设定速度
  
  
   display();                                     //液晶显示
   
  
  
  // deal_key_event();                              //处理按键                              
  
 

  led_turn (LED1);                                  
  ImgNum++;
  
}






