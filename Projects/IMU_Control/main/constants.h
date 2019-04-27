////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define EEPROM_SIZE 0x64

#define BEGINING_ADDR         0x00

#define BNO055_CHIP_ID	      0x00
#define BNO055_ACC_ID         0x01
#define BNO055_PAGE_ID        0x07

#define BNO055_ACC_DATA_X_LSB 0x08
#define BNO055_ACC_DATA_X_MSB 0x09

#define BNO055_EUL_YAW_LSB    0x1A
#define BNO055_EUL_YAW_MSB    0x1B
#define BNO055_EUL_ROLL_LSB   0x1C
#define BNO055_EUL_ROLL_MSB   0x1D
#define BNO055_EUL_PITCH_LSB  0x1E
#define BNO055_EUL_PITCH_MSB  0x1F
#define BNO055_UNIT_SEL       0x3B
#define BNO055_OPR_MODE       0x3D
#define BNO055_PWR_MODE       0x3E
#define BNO055_SYS_TRIGGER    0x3F

//#define IMU_ADDRESS_ROTUNDA   0x2B
#define IMU_ADDRESS_SHOULDER  0x29
//#define IMU_ADDRESS_ELBOW     0x2A
//#define IMU_ADDRESS_WRIST     0x28
