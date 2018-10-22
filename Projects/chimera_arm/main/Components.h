#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <stdlib.h>

using namespace std;
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
//#include "Protolobe.h"
//#include "RTOS_Tasks.h"
//#include "ArmDefines.h"

#include "Armstructs.h"
#include "driver/ledc.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/i2c.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h" 
extern "C"{
#include "Wifi.h"
#include "Structure.h"
}
xSemaphoreHandle print_mux;
//static xQueueHandle wifi_queue = xQueueCreate(4 ,sizeof(WifiEvent));

current_pos_t current_pos;


extern "C" void vReadDataTask(void *pvParameters);

extern "C" void vSendDataTask(void *pvParameters);

//extern "C" void error_task(void *pvParameters);

void Arm_claw_task(void *pvParameters);

void Arm_elbow_task(void *pvParameters);

void Arm_cam_task(void *pvParameters);

void Arm_wrist_task(void *pvParameters);

void Arm_shoulder_task(void *pvParameters);

void Gimbal_task(void *pvParameters);
void Arm_rotunda_task(void *pvParameters);

class GimbalController{
public:

void init();
int getData();
void sendData(int data);
private:

};







class CameraController{
public:
CameraController( int cameraID, int PitchAngle){
current_cameraID = cameraID;
CurrentPitchAngle = PitchAngle;
}
void SwitchCamera(int ID);		//discuss with paul/dmitri
void moveCamera(int degrees);
private:
int current_cameraID;
int CurrentPitchAngle;
};




class ArmComponent{
public:
void notifyTimeout();
protected:
TimeOut_t xTimeOut;

char name[40]; 
int status;	
};





class ArmWrist: public ArmComponent{
public:
ArmWrist(char* id){
quad = 0;
count = 0; 
currentPitchAngle = 0;
desiredPitchAngle = 0;
currentRotationAngle = 180;
desiredRotationAngle = 180;
mode_gimble_on = 0;
strcpy(name,id);
}
void move(int rot_degrees, int pitch_degrees);
void setgimbalon();
void setgimbaloff();
void getPitchPosition();
void getRotationPosition();
private:
int desiredPitchAngle;
int currentPitchAngle;
int desiredRotationAngle;
int currentRotationAngle;
bool mode_gimble_on;		
int16_t count;
uint8_t quad;
};





class ArmBase: public ArmComponent{
public:
ArmBase(char* id){
currentAngle = 0;
strcpy(name,id);
}
void moveTo(int degrees);
private:
int currentAngle;
};






class ArmShoulder: public ArmComponent{
public:
ArmShoulder(char* id){
currentAngle = 90;
desiredAngle = 90;
strcpy(name,id);
}
void move(int degrees);
void getAngleFromMagEnc();
private:
int desiredAngle;
int currentAngle;
};





class ArmClaw: public ArmComponent{
public:
ArmClaw(char* id){
torque = 50;
strcpy(name,id);
}
void setTorque(int num);
void open();
void Close();
private:
int torque;
};




class ArmElbow: public ArmComponent{
public:
ArmElbow(char* id){
currentAngle = 0;
strcpy(name,id);
}
void moveTo(int degrees);
private:
int currentAngle;
};
#endif
