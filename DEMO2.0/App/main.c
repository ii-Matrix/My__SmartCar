/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"



/*
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
 */
void  main(void)
{
    Init_All(); 
    while(1)
    {
     
     
        camera_get_img();                                   //����ͷ��ȡͼ��
       
        //�ڰ�����ͷ
     //   LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
       // vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
}




void My_Run()
{
   img_extract((uint8*)Img,ImgAgo,CAMERA_SIZE);    //��ѹ��,һά������ά����
   StartLineCheck();                               //�����߼��
  
   BottomFind();                                   //�ײ�Ѱ��
   GenZong();                                      //���ٱ�Ե  

   ForeSight();                                    //ȷ��ǰհ

   jisuan_Middle();                                //��������

  judge_road();                                    //�ж���������         
  MiddleError = jisuan_piancha();                   //����ƫ��
  AbsMiddleError = Myabsfloat(MiddleError); 
  

  Direct_PD();
  SpeedSet();                                     //�����趨�ٶ�
  
  
   display();                                     //Һ����ʾ
   
  
  
  // deal_key_event();                              //������                              
  
 

  led_turn (LED1);                                  
  ImgNum++;
  
}






