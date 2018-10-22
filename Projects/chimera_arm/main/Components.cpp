#include "Components.h"


extern "C" void vReadDataTask(void *pvParameters){
  
  while(1){
    
     vTaskDelay(100);
     //read data from mission control. 
     read_data_wifi(READ_ITEM_SIZE);
     printf("read data rotunda: %i\n", read_data.data[read_rotunda]);
     printf("read data shoulder: %i\n", read_data.data[read_shoulder]);
     printf("read data elbow: %i\n", read_data.data[read_elbow]);
     printf("read data wrist pitch: %i\n", read_data.data[read_wrist_pitch]);
     printf("read data wrist rotation: %i\n", read_data.data[read_wrist_rotation]);
     printf("read data cam: %i\n", read_data.data[read_cam]);

   }



}

extern "C" void vSendDataTask(void *pvParameters)
{
	while(1)
	{
		send_data.data[send_rotunda] = current_pos.base;
		send_data.data[send_shoulder] = current_pos.shoulder;
		send_data.data[send_elbow] = current_pos.elbow;
		send_data.data[send_wrist_pitch] = current_pos.wrist_pitch;
		send_data.data[send_wrist_rotation] = current_pos.wrist_rotation;
		send_data_wifi(SEND_ITEM_SIZE);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
/*
extern "C" void error_task(void *pvParameters){

}
*/

void Arm_claw_task(void *pvParameters){
    ArmClaw claw("claw");
    while(1)
	{
        
	    	if(read_data.data[read_claw_motion] == 2)
		{
	        	if(read_data.data[read_torque] <0 || read_data.data[read_torque] > 100)
			{
	        	    printf("Error. Invalid torque: %i \n", read_data.data[read_torque]);
	        	    //error
	        	}
	        	else
			{
	        	claw.setTorque(read_data.data[read_torque]);
	        	}
		    claw.open();
		}
    else if(read_data.data[read_claw_motion] == 1)
    {
        if(read_data.data[read_torque] <0 || read_data.data[read_torque] > 100)
	{
            printf("Error. Invalid torque: %i \n", read_torque);
            //error
        }
        else
	{
	        claw.setTorque(read_data.data[read_torque]);
        }
        claw.Close();
    }
}
}

void Arm_elbow_task(void *pvParameters){
    ArmElbow elbow("elbow");
    while(1){
         
elbow.moveTo(read_data.data[read_elbow]);
    }
}

void Arm_cam_task(void *pvParameters){
    CameraController cameracontroller(0, 0);
    while(1){
        cameracontroller.SwitchCamera(0); 
        cameracontroller.moveCamera( read_data.data[read_cam_shoulder]);
        cameracontroller.SwitchCamera(1); 
        cameracontroller.moveCamera( read_data.data[read_cam_elbow]);
        vTaskDelay(250 / portTICK_RATE_MS);  
    }
}

void Arm_wrist_task(void *pvParameters){
    ArmWrist wrist("wrist");

    while(1){
        if(read_data.data[read_gimbal]){
    	wrist.setgimbalon();
	}
else{
    wrist.setgimbaloff();       
    }
    wrist.move(read_data.data[read_wrist_rotation], read_data.data[read_wrist_pitch]); 
    vTaskDelay(250 / portTICK_RATE_MS);  
}
}

void Arm_shoulder_task(void *pvParameters){
 ArmShoulder shoulder("shoulder");
 while(1){
     shoulder.move(read_data.data[read_shoulder]);
     vTaskDelay(250 / portTICK_RATE_MS); 
 }   
}

void Arm_rotunda_task(void *pvParameters){
 ArmBase base("base");
 int angle;
 while(1){
    base.moveTo(read_data.data[read_rotunda]);  
    vTaskDelay(250 / portTICK_RATE_MS); 
 }
}

void Gimbal_task(void *pvParameters){
GimbalController gimbal;
gimbal.init();
int data;
while(1){
	
	data = gimbal.getData();
	gimbal.sendData(data);
	
	vTaskDelay(100 / portTICK_RATE_MS);
	//error?   
}
}



void CameraController::SwitchCamera(int ID){
if(current_cameraID != ID){
gpio_set_level(CAM_MUX_SEL, ID);  //0 or 1
current_cameraID = ID;
}
}
void CameraController::moveCamera( int degrees){
if(degrees < -90 || degrees > 90){
     printf("Error. Invalid camera angle: %i\n", degrees);
     //error
 }  
  else{
      int temp = (degrees+90)+205;
ledc_set_duty(CAM_CHANNEL.speed_mode, CAM_CHANNEL.channel, temp);
ledc_update_duty(CAM_CHANNEL.speed_mode, CAM_CHANNEL.channel);
  }
}



void ArmComponent::notifyTimeout(){
printf("Error, %s timed out\n", name);
//error
}




void ArmWrist::move(int rot_degrees, int pitch_degrees){

    if(rot_degrees < 0 || rot_degrees > 359 || pitch_degrees < -150 || pitch_degrees > 150){
     printf("Error. Invalid wrist angle: rot: %i  pitch: %i\n", rot_degrees, pitch_degrees);
     //error
 }  
 else{
desiredPitchAngle = pitch_degrees;
desiredRotationAngle = rot_degrees;
vTaskSetTimeOutState( &xTimeOut );
TickType_t xTicksToWait = 1000;
getPitchPosition();

while(abs(currentPitchAngle - desiredPitchAngle) > 5){	

if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
      		{
		notifyTimeout();
		break;
		}
if(currentPitchAngle < desiredPitchAngle){	//forward
ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 1023);
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 512); 
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);

}
else{						//reverse
ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 512); 
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 1023);
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);

}
getPitchPosition();
}

ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 1023); //brakes
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 1023); 
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);

if(mode_gimble_on){
        //Gimbal adjustment
    int data;  
    if(!xQueueReceive(queue_handle, &data, 200)){
printf("failed to receive data");
//report error
}                          
int temp = currentPitchAngle;
currentPitchAngle = data;
desiredPitchAngle = (temp-currentPitchAngle)+ temp;

while(currentPitchAngle != desiredPitchAngle){	                    //PITCH
if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
      		{
		notifyTimeout();
		break;
		}
if(currentPitchAngle < desiredPitchAngle){	//forward
ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 1023);
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 512); 
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);

}
else{						//reverse
ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 512); 
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 1023);
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);

}
getPitchPosition();
}
ledc_set_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel, 1023); //brakes
ledc_update_duty(PITCH_CHANNEL1.speed_mode, PITCH_CHANNEL1.channel);
ledc_set_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel, 1023); 
ledc_update_duty(PITCH_CHANNEL2.speed_mode, PITCH_CHANNEL2.channel);	
}

getRotationPosition();                                              //ROTATION
	while(currentRotationAngle != desiredRotationAngle){
	if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
      		{
		notifyTimeout();
		break;
		}
	if(currentRotationAngle < desiredRotationAngle){	//forward
ledc_set_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel, 1023); 
ledc_update_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel);
ledc_set_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel, 512);   
ledc_update_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel);

}
else{						//reverse
ledc_set_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel, 512); 
ledc_update_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel);
ledc_set_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel, 1023);   
ledc_update_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel);

}
getRotationPosition(); 
}
ledc_set_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel, 1023);     //brakes
ledc_update_duty(ROT_CHANNEL1.speed_mode, ROT_CHANNEL1.channel);
ledc_set_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel, 1023);   
ledc_update_duty(ROT_CHANNEL2.speed_mode, ROT_CHANNEL2.channel);

}

}
void ArmWrist::getRotationPosition(){
quad <<= 2;
   quad |= gpio_get_level(WRIST_ENCODER_A) & 0x01;
    quad |= (gpio_get_level(WRIST_ENCODER_B) << 1) & 0x02;

   const int8_t state[16] = {
                                 0,    // 00 00
                                 -1,   // 00 01
                                 1,    // 00 10
                                 0,    // 00 11
                                 1,    // 01 00
                                 0,    // 01 01
                                 0,    // 01 10
                                 -1,   // 01 11
                                 -1,   // 10 00
                                 0,    // 10 01
                                 0,    // 10 10
                                 1,    // 10 11
                                 0,    // 11 00
                                 1,    // 11 01
                                 -1,   // 11 10
                                 0     // 11 11
                              };

   printf("quad: %X state: %i ", quad, (quad & 0x0F));
   count += state[(quad & 0x0F)];

currentRotationAngle = (count/550)*360;
}
void ArmWrist::getPitchPosition(){
int pitchAngle;
if(!xQueueReceive(queue_handle, &pitchAngle, 1000)){
printf("failed to receive data");
}
currentPitchAngle = pitchAngle;
}
void ArmWrist::setgimbalon(){
    mode_gimble_on = 1;
}
void ArmWrist::setgimbaloff(){
    mode_gimble_on = 0;
}


void ArmBase::moveTo(int degrees){
    
 if(degrees < -150 || degrees > 150){
     printf("Error. Invalid rotunda angle: %i\n", degrees);
     //error
 }  
  else{
      int temp = (((degrees+150)*818)/360)+205;
ledc_set_duty(BASE_CHANNEL.speed_mode, BASE_CHANNEL.channel, temp);
ledc_update_duty(BASE_CHANNEL.speed_mode, BASE_CHANNEL.channel);
  }
}




void ArmShoulder::move(int degrees){
if(degrees<0 || degrees>180){
printf("Error. Invalid shoulder angle: %i \n", degrees);
//error
}
else{
desiredAngle = degrees;
getAngleFromMagEnc();
vTaskSetTimeOutState( &xTimeOut );
TickType_t xTicksToWait = 1000;
while(currentAngle != desiredAngle){
if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
      		{
		notifyTimeout();
		break;
		}
if(currentAngle < desiredAngle){
    gpio_set_level(SHOULDER_DIR_PIN, 1);
ledc_set_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel, 512); //forward
ledc_update_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel);

}
else{
    
    gpio_set_level(SHOULDER_DIR_PIN, 0);
ledc_set_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel, 512); //reverse
ledc_update_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel);

}
getAngleFromMagEnc();
}
}
ledc_set_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel, 0); 
ledc_update_duty(SHOULDER_CHANNEL.speed_mode, SHOULDER_CHANNEL.channel);
}
void ArmShoulder::getAngleFromMagEnc(){
    uint8_t data[6];
int16_t X, Y, Z;
int16_t thetaX, thetaY, thetaZ;
esp_err_t ret;
i2c_cmd_handle_t a_read; 
//printf("\nWRITE ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, AS5600_SENSOR_ADDR<< 1 | WRITE_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_read, 0x0C, ACK_EN);
    //printf("reg 1: %X ", ret);
    ret = i2c_master_stop(a_read);
   // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_read);
 
    // printf("\n");
 
    //printf("READ ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, AS5600_SENSOR_ADDR<< 1 | READ_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_read_byte(a_read, data, ACK_VAL);
   // printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 1), ACK_VAL);
   // printf("data L: %X ", ret);
	ret = i2c_master_read_byte(a_read, (data + 2), ACK_VAL);
    //printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 3), NACK_VAL);
    //printf("data L: %X ", ret);
    ret = i2c_master_stop(a_read);
    //printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %i \n", ret);
    i2c_cmd_link_delete(a_read);
    
    
X = 0;//fix
Y = 0;
Z = 0;
	X = ((int16_t)data[0] << 8) | data[1];
Y = ((int16_t)data[2] << 8) | data[3];
Z = ((int16_t)data[4] << 8) | data[5];
thetaX = X/23;
thetaZ = Z/23;
currentAngle = thetaZ;
}




void ArmClaw::open(){
int temp = (torque*1023)/100;

while(read_data.data[read_claw_motion] == 2){  
    ledc_set_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel, 1023);
ledc_update_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel);
ledc_set_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel, 1023- temp); 
ledc_update_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel);
}
   ledc_set_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel, 1023);
ledc_update_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel);
ledc_set_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel, 0); 
ledc_update_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel);

}
void ArmClaw::Close(){
int temp = (torque*1023)/100;

while(read_data.data[read_claw_motion] == 1){  
 ledc_set_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel, 1023 - temp);
ledc_update_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel);
ledc_set_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel, 1023); 
ledc_update_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel);
}
 ledc_set_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel, 0);
ledc_update_duty(CLAW_CHANNEL1.speed_mode, CLAW_CHANNEL1.channel);
ledc_set_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel, 1023); 
ledc_update_duty(CLAW_CHANNEL2.speed_mode, CLAW_CHANNEL2.channel);

}




void ArmElbow::moveTo(int degrees){
if(degrees < -150 || degrees > 150){
     printf("Error. Invalid elbow angle: %i\n", degrees);
     //error
 }  
    int temp = (((degrees+150)*818)/360)+205;
ledc_set_duty(ELBOW_CHANNEL.speed_mode, ELBOW_CHANNEL.channel, temp); 
ledc_update_duty(ELBOW_CHANNEL.speed_mode, ELBOW_CHANNEL.channel); 
}


void GimbalController::init(){
print_mux = xSemaphoreCreateMutex();
    esp_err_t ret;
    i2c_example_master_init();
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BNO055_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, OPR_MODE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x0C, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,BNO055_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, UNIT_SEL, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);


}
int GimbalController::getData(){
uint8_t data[6];
int16_t X, Y, Z;
int16_t thetaX, thetaY, thetaZ;
esp_err_t ret;
    i2c_cmd_handle_t a_read;
    


    //printf("\nWRITE ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, BNO055_SENSOR_ADDR<< 1 | WRITE_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_read, 0x1A, ACK_EN); //starting address for euler angles
    //printf("reg 1: %X ", ret);
    ret = i2c_master_stop(a_read);
   // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_read);
 
    // printf("\n");
 
    //printf("READ ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, BNO055_SENSOR_ADDR<< 1 | READ_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_read_byte(a_read, data, ACK_VAL);
   // printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 1), ACK_VAL);
   // printf("data L: %X ", ret);
	ret = i2c_master_read_byte(a_read, (data + 2), ACK_VAL);
    //printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 3), ACK_VAL);
   // printf("data L: %X ", ret);
	ret = i2c_master_read_byte(a_read, (data + 4), ACK_VAL);
    //printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 5), NACK_VAL);
    //printf("data L: %X ", ret);
    ret = i2c_master_stop(a_read);
    //printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %i \n", ret);
    i2c_cmd_link_delete(a_read);
    
    

	X = ((int16_t)data[1] << 8) | data[0];
Y = ((int16_t)data[3] << 8) | data[2];
Z = ((int16_t)data[5] << 8) | data[4];
thetaX = X/16;      //euler angles = 1 degree per 16 ticks
thetaY = Y/16;
thetaZ = Z/16;
printf("X:%i, Y:%i, Z:%i\n", thetaX, thetaY, thetaZ);
return thetaX;     
}
void GimbalController::sendData(int data){
if(! xQueueSend(queue_handle, &data, 200)){
printf("failed to send data");
//report error
}
}


