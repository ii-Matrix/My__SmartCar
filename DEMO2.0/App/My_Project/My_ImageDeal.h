#ifndef MY_IMAGEDEAL_H
#define MY_IMAGEDEAL_H

#include  "include.h"
#include "common.h"

void BottomFind();
void GenZong();
void jisuan_Middle();
uint8 Refind_L(uint8 y);//再找一次左边界
uint8 Refind_R(uint8 y);
void ForeSight();
void judge_road();
int  judge_crossxieR(uint8 judge);
int judge_crossxieL(uint8 judge);
float jisuan_piancha();
void StartLineCheck();
void StraightCheck();
void OutsideCheck();
void CrossCheck();




#endif
