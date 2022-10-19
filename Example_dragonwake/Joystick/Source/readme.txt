LandTiger Joystick Example

Example functionality:                                                   
 - Clock Settings:
   - XTAL    =           12.00 MHz
   - SYSCLK  =          100.00 MHz

-- CMSIS ---------------------------------------------
/****************************************************************/
Joystickのテスト。
システムクロックは100MHz。
JP8をショートしてLEDをEnableにする必要。
試験結果：
　五方向のJoystickの押下状態をボードのLD7~LD11に反映する。
