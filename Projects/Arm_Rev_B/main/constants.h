////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once
#define EEPROM_SIZE 0x64

#define BEGINING_ADDR 0x00

#define BNO055_CHIP_ID     0x00
#define BNO055_ACC_ID      0x01
#define BNO055_PAGE_ID     0x07
#define BNO055_ADDR0       0x28
#define BNO055_ADDR1       0x29
#define BNO055_OPR_MODE    0x3D
#define BNO055_PWR_MODE    0x3E
#define BNO055_SYS_TRIGGER 0x3F

#define act_PHASE 0
#define act_ENABLE 2

/* PINOUTS:
	Servo 1: GPIO 6
	Servo 2: GPIO 7

	i2c:
		SDA: GPIO 23
		SCL: GPIO 21

	Motors:
		Motor Driver 1:	DIFF GEARBOX
			VPROPI: GPIO 39
			Mode 2: GPIO 27
			Phase:	GPIO 16
			Enable:	GPIO 4

		Motor Driver 2:	DIFF GEARBOX
			VPROPI: GPIO 36
			Mode 2: GPIO 1
			Phase:	GPIO 5
			Enable:	GPIO 17

		Motor Driver 3:
			VPROPI: GPIO 34
			Mode 2: GPIO 3
			Phase:	GPIO 0
			Enable:	GPIO 2
*/

constexpr int kRotundaPin = 18;		//Servo 1
constexpr int kRotundaPosMin = 0;
constexpr int kRotundaPosmax = 3600;
constexpr double kRotundaStartPos = 1800;
constexpr double kRotundaStartDuty = 50.0;
constexpr double kRotundaFreq = 50;
constexpr double kRotundaPWMMin = 5;
constexpr double kRotundaPWMMax = 10;

constexpr int kElbowPin = 25;		//Servo 2
constexpr int kElbowLimitMin = 0;
constexpr int kElbowLimitMax = 300;
constexpr double kElbowRange = 300;
constexpr double kElbowStartPos = 150;
constexpr double kElbowFreq = 50;
constexpr double kElbowPWMMin = 2.5;
constexpr double kElbowPWMMax = 12.5;


constexpr uint32_t kMotorFreq = 5000;
constexpr uint32_t kShoulderSigPin = 33;
constexpr uint32_t kShoulderDirPin = 32;
constexpr uint32_t kShoulderEnablePWMMin = 0;	//percentage
constexpr uint32_t kShoulderEnablePWMMax = 50;	//percentage

constexpr uint32_t kWristLeftSigPin = 4;
constexpr uint32_t kWristLeftDirPin = 16;

constexpr uint32_t kWristRightSigPin = 17;
constexpr uint32_t kWristRightDirPin = 5;	//port 2
