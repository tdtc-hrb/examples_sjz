This example project uses CMSIS and demonstrates 
the use of Systick in interrupt driven mode

LandTiger GPIO Example

Example functionality:                                                   
 - Clock Settings:
   - XTAL    =           12.00 MHz
   - SYSCLK  =          100.00 MHz

-- CMSIS ---------------------------------------------
SystemInit       called from startup_LPC17xx.s
SystemCoreClock  is 100000000Hz
SysTick          runs in interrupt mode

JP8 jumper 
			Open:   LED disabled    
			Short:	LED active (default)
/****************************************************************/
SYStickのタイマー割り込みモードで遅延処理を実現。
システムクロックは100MHz。
JP8をショートしてLEDをEnableする必要。
試験結果：
　8個のLEDを一つづつ100ms点灯する。

