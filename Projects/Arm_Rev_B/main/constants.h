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

#define act_PHASE 5
#define act_ENABLE 17



constexpr int kRotundaPin = 18;		//Servo 1
constexpr int kRotundaPosMin = 0;
constexpr int kRotundaPosmax = 3600;
constexpr double kRotundaStartPos = 1800;
constexpr double kRotundaStartDuty = 50.0;
constexpr double kRotundaFreq = 50;
constexpr double kRotundaPWMMin = 5;
constexpr double kRotundaPWMMax = 10;

constexpr int kElbowPin = 27;		//Servo 2
constexpr int kElbowLimitMin = 0;
constexpr int kElbowLimitMax = 300;
constexpr double kElbowRange = 300;
constexpr double kElbowStartPos = 150;
constexpr double kElbowFreq = 50;
constexpr double kElbowPWMMin = 2.5;
constexpr double kElbowPWMMax = 12.5;


constexpr uint32_t kMotorFreq = 5000;
constexpr uint32_t kShoulderSigPin = 25;
constexpr uint32_t kShoulderDirPin = 26;
constexpr uint32_t kShoulderEnablePWMMin = 0;	//percentage
constexpr uint32_t kShoulderEnablePWMMax = 50;	//percentage

constexpr uint32_t kWristLeftSigPin = 0;
constexpr uint32_t kWristLeftDirPin = 2;

constexpr uint32_t kWristRightSigPin = 16;
constexpr uint32_t kWristRightDirPin = 4;	//port 2
