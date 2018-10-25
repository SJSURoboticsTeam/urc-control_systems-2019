#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Arm.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////
    bool setMotorPosition(uint16_t orientation, MOTOR_TYPE motor_to_set);   

    motor_info_struct.Current_Orientation getMotorPosition(MOTOR_TYPE motor_to_read);
    //Figure out this type ^

    void rotateWrist(uint16_t degrees);
   
    void openClaw(void);

    void closedClaw(void);

    void laserOn(void);
        
    void laserOff(void);

    void returnToOrigin(void);
        
    bool grabPOD(void);
           
    bool deployPod(void);

    bool storePOD(void)

    bool updateIMU(IMU imu_to_update);

    bool readMagneticEncoder();

    uint32_t getClawCurrent(void);

    bool sendToMC(motor_info_struct*, IMU_Data*, Magenetic_Encoder*);

////////////////////////////////////////////////////////////////////////////////
//                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
