/****************************************************************************
 *   $Id:: adctest.c 4085 2010-07-30 22:31:04Z usb00423                     $
 *   Project: NXP LPC11xx ADC example
 *
 *   Description:
 *     This file contains ADC test modules, main entry, to test ADC APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "adc.h"
#if ADC_DEBUG
#include "uart.h"
#endif

extern volatile uint32_t ADCValue[ADC_NUM];
extern volatile uint32_t ADCIntDone;
extern volatile uint32_t OverRunCounter;

volatile uint32_t ADCMinValue[ADC_NUM];
volatile uint32_t ADCMaxValue[ADC_NUM];
volatile uint32_t ADCConversionCounter = 0;
volatile uint32_t ADCConversionBurstCounter = 0;

#if ADC_DEBUG
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

uint8_t ConvertDigital ( uint8_t digital )
{
  uint8_t ascii_char;

  if ( (digital >= 0) && (digital <= 9) )
  {
	ascii_char = digital + 0x30;	/* 0~9 */
  }
  else
  {
	ascii_char = digital - 0x0A;
	ascii_char += 0x41;				/* A~F */
  }
  return ( ascii_char );
}

/*****************************************************************************
** Function name:		PrintoutADCValue
**
** Descriptions:		Dump ADC reading to the UART
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void PrintoutADCValue( void )
{
  uint32_t i;

  for ( i = 0; i < ADC_NUM; i++ )
  {
//	LPC_UART->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	UARTBuffer[0] = ConvertDigital( (uint8_t)(ADCValue[i]>>28));
	UARTBuffer[1] = ConvertDigital( (uint8_t)((ADCValue[i]>>24)&0xF));
	UARTBuffer[2] = ConvertDigital( (uint8_t)((ADCValue[i]>>20)&0xF));
	UARTBuffer[3] = ConvertDigital( (uint8_t)((ADCValue[i]>>16)&0xF));
	UARTBuffer[4] = ConvertDigital( (uint8_t)((ADCValue[i]>>12)&0xF));
	UARTBuffer[5] = ConvertDigital( (uint8_t)((ADCValue[i]>>8)&0xF));
	UARTBuffer[6] = ConvertDigital( (uint8_t)((ADCValue[i]>>4)&0xF));
	UARTBuffer[7] = ConvertDigital( (uint8_t)(ADCValue[i]&0xF));
	UARTBuffer[8] = '\r';
	UARTBuffer[9] = '\n';
	UARTSend( (uint8_t *)UARTBuffer, 10 );
//	LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
  }
  return;
}
#endif

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i;
  
  SystemInit();

#if ADC_DEBUG  
  /* NVIC is installed inside UARTInit file. */
  UARTInit(115200);
#endif

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADCMinValue[i] = 0x03FF;
	ADCMaxValue[i] = 0x0000;
  }

  /* Initialize ADC  */
  ADCInit( ADC_CLK );

  while ( 1 )
  {
#if BURST_MODE				/* Interrupt driven only */
	/* Burst mode */
	ADCBurstRead();
	while ( !ADCIntDone );
	ADCIntDone = 0;
	if ( OverRunCounter != 0 )
	{
	  while ( 1 );
	}
	ADCConversionBurstCounter++;
	for ( i = 0; i < ADC_NUM; i++ )
	{
  	  if ( ADCValue[i] <= ADCMinValue[i] )
	  {
		ADCMinValue[i] = ADCValue[i];
	  }
	  if ( ADCValue[i] >= ADCMaxValue[i] )
	  {
		ADCMaxValue[i] = ADCValue[i];
	  }
	}
#else
	/* Non burst mode, get ADC from all channels. */
	for ( i = 0; i < ADC_NUM; i++ )
	{
#if ADC_INTERRUPT_FLAG				/* Interrupt driven */
	  ADCRead( i );
	  while ( !ADCIntDone );
	  ADCIntDone = 0;
	  if ( OverRunCounter != 0 )
	  {
		while ( 1 );
	  }
#else
	  ADCValue[i] = ADCRead( i );	/* Polling */
#endif
	  ADCConversionCounter++;
	  if ( ADCValue[i] <= ADCMinValue[i] )
	  {
		ADCMinValue[i] = ADCValue[i];
	  }
	  if ( ADCValue[i] >= ADCMaxValue[i] )
	  {
		ADCMaxValue[i] = ADCValue[i];
	  }
	}
#endif
#if ADC_DEBUG
	PrintoutADCValue();
#endif
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/

