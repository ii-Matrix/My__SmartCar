#ifndef MY_MATH_H
#define MY_MATH_H



#include "include.h"
#include  "common.h"

float jisuan_xielv(uint8 line[60],uint8 xielvstart,uint8 xielvstop);   //最小二乘法
float jisuan_xielv_orginal(uint8 line[60],uint8 xielvstart,uint8 xielvstop); //最小二乘法
float jisuan_qulv(uint8 temp_x1,uint8 temp_y1,uint8 temp_x2,uint8 temp_y2,uint8 temp_x3,uint8 temp_y3);
float Myabsfloat(float a);
int jisuan_area(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3);
float Myabsfloat(float a);
int jisuan_area(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3);
uint32 abs_sub(uint32 diff1,uint32 diff2);


#endif
