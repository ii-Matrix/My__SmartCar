#ifndef MY_SPEED_H
#define MY_SPEED_H

#include "include.h"
#include "common.h"


void SpeedSet(void);
void SpeedSet_2(void);

void Speed_Measure(void);
extern void MotorOut(void);
extern void MotorOut2(void);
void Speed_PID();
void  Speed_PID_2();

void DistanceRecord();





#endif
