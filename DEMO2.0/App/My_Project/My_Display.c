#include "My_Display.h"


int16 page = 0,LastPage = 0;                    //页数
int16 Img_LCD_Loaction_X=1;          //图像在LCD上的位置
int16 Img_LCD_Loaction_Y=24;


/****************************
作用：液晶显示总函数
****************************/
void display()
{
  Site_t site;
  int YuZhi = 150;
/*  if((CarState == Car_Stop ))
  {
    LCD_clear(RED);
  }*/
 
    
    if(page ==0 )
    {
      site.x=site.y=0;
      GiveColor();
      LCD_Camera();
      site.x=64;
      site.y=48;
      LCD_cross(site,24,RED);
      
      site.y=96;
      site.x=0;
      if(MiddleError>=0)
        LCD_char(site,'+', BLUE,RED);
      else 
        LCD_char(site,'-', BLUE,RED); 
      site.x=8;
      LCD_num_BC(site,(int)(AbsMiddleError*10),4, BLUE,RED);
      
      site.y=112;
      site.x=0;
      if(Pwm_out >= Pwm_mid)
        LCD_char(site,'+', BLUE,RED);
      else 
        LCD_char(site,'-', BLUE,RED); 
      
      site.x=8;
      LCD_num_BC(site,abs_sub(Pwm_out,Pwm_mid),4, BLUE,RED);
      
      site.y=112;
      site.x=40;
      LCD_num_BC(site,(int)(Myabsfloat(Error_Pwm)),3, BLUE,RED);
      
      site.y=96;
      site.x=40;
      LCD_num_BC(site,Sight,3, BLUE,RED);
      
    
      
      site.x = 88;
      LCD_num_BC(site,SpeedGoal,3, BLUE,RED);
   
    }
   
  
}

/****************************
作用：在液晶上显示图像
****************************/
void LCD_Camera()
{
  uint8 x,y;
  Site_t site;
  Size_t size;
  site.x=site.y=0;
  size.W=128;size.H=96;
  LCD_PTLON(site, size);             //开窗
  LCD_RAMWR();                       //写内存 
  
  for(y=Img_LCD_Loaction_Y;y<96+Img_LCD_Loaction_Y;y++)
  {
    for(x=Img_LCD_Loaction_X*16;x<128+Img_LCD_Loaction_X*16;x++)
    {              
      if(Img[y][x]==ImgWhite)
        LCD_WR_DATA(WHITE);
      
      if(Img[y][x]==ImgBlack)
          LCD_WR_DATA(BLACK);
      
      if(Img[y][x]==ImgGreen)
         LCD_WR_DATA(GREEN);
      
      if(Img[y][x]==ImgBlue)
          LCD_WR_DATA(BLUE); 
      
      if(Img[y][x]==ImgRed)
          LCD_WR_DATA(RED);        
    }
  } 
}

/*********************************************
作用：改变数组相应位置的值，为液晶显示做准备
*********************************************/
void GiveColor()
{
  for(int y=119;y>=Sight_L;y--)
    Img[y][Left[y]]=ImgBlue;
  for(int y=119;y>=Sight_R;y--)
    Img[y][Right[y]]=ImgRed;
  for(int y=119;y>=Sight;y--)
  {
  
    {

      if((Lost_L[y]==LostWhite && Lost_R[y]==LostWhite) || (Lost_L[y]==LostBlack && Lost_R[y]==LostBlack))
      {
        
      }
      else
        Img[y][Middle[y]]=ImgGreen;   
  
    }
  }
}


