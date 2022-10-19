/****************************************Copyright (c)****************************************************
**                                 http://www.PowerAVR.com
**								   http://www.PowerMCU.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2010-05-12
** Last Version:        V1.00
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          PowerAVR
** Created date:        2010-05-10
** Version:             V1.00
** Descriptions:        编写示例代码
**
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#include "LPC17xx.h"

volatile uint16_t GusSinTable[45] =                                       /* 正弦表                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/*****************************************************************************
** Function name:		DACInit
**
** Descriptions:		initialize DAC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void DACInit( void )
{
  /* setup the related pin to DAC output */
  LPC_PINCON->PINSEL1 = 0x00200000;	/* set p0.26 to DAC output */   
  return;
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        DA在P0.26引脚上输出正弦信号，实验时请用示波器观察
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main(void)
{
	uint8_t i;
	SystemInit();		   /* 初始化目标板，切勿删除       */                                                                            
    
	
  	DACInit();			  /* Initialize DAC  */

    while (1) 
	{
     	for (i = 0; i < 45; i++)
		{	
    	    LPC_DAC->DACR = (GusSinTable[i] << 6);                               /* 输出正弦波                   */
    	}
    } 
}
