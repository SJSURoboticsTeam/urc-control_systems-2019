#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED

#ifdef _cplusplus
extern "C"{
#endif

#define SEND_ITEM_SIZE 6
#define READ_ITEM_SIZE 11

extern const char* send_data_label[];

typedef enum {
    // send
    send_rotunda=0,
    send_shoulder=1,
    send_elbow=2,
    send_wrist_pitch=3,
    send_wrist_rotation=4,
    send_cam=5,
    
    // read
    read_cam = 0,
    read_cam_shoulder = 1, 
    read_cam_elbow = 2,
    read_gimbal = 3,
    read_rotunda = 4,
    read_shoulder = 5,
    read_elbow = 6,
    read_wrist_pitch = 8,
    read_wrist_rotation = 7,
    read_claw_motion = 9,
    read_torque = 10

};

typedef struct {
	int base;
	int shoulder;
	int elbow;
	int wrist_pitch;
	int wrist_rotation;
	int claw;
}current_pos_t;

typedef struct{
    int data[SEND_ITEM_SIZE]; //set array size as number of send data item
}Send_Data;

typedef struct {
    int data[READ_ITEM_SIZE]; //set array size as number of read data item
}Read_Data;

extern Send_Data send_data;
extern Read_Data read_data;
#ifdef _cplusplus
}
#endif

#endif /* Wifi */
