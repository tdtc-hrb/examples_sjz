/****************************************************************************
 *   $Id:: gpiotest.c 3635 2010-06-02 00:31:46Z usb00423                    $
 *   Project: NXP LPC11xx GPIO example
 *
 *   Description:
 *     This file contains GPIO test modules, main entry, to test GPIO APIs.
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
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "gpio.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();

  GPIOInit();

  /* use port0_1 as input event, interrupt test. */
  GPIOSetDir( PORT0, 1, 0 );
  /* port0_1, single trigger, active high. */
  GPIOSetInterrupt( PORT0, 1, 0, 0, 0 );
  GPIOIntEnable( PORT0, 1 );

  /* use port1_1 as input event, interrupt test. */
  GPIOSetDir( PORT1, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIOSetInterrupt( PORT1, 1, 0, 0, 0 );
  GPIOIntEnable( PORT1, 1 );

  /* use port2_1 as input event, interrupt test. */
  GPIOSetDir( PORT2, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIOSetInterrupt( PORT2, 1, 0, 0, 0 );
  GPIOIntEnable( PORT2, 1 );

  /* use port3_1 as input event, interrupt test. */
  GPIOSetDir( PORT3, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIOSetInterrupt( PORT3, 1, 0, 0, 0 );
  GPIOIntEnable( PORT3, 1 );

  while( 1 );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
