#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED

#ifdef _cplusplus
extern "C"{
#endif

#define SEND_ITEM_SIZE 8
#define READ_ITEM_SIZE 11

extern const char* send_data_label[];

typedef enum {
    // send
    send_current=0,

    send_shoulder_rotation=1,
    send_elbow_rotation=2,
    
    send_rotunda=3,
    send_shoulder=4,
    send_elbow=5,
    send_wrist_pitch=6,
    send_wrist_rotation=7,
    
    // read
    read_cam_id = 0,
    read_cam_shoulder = 1,
    read_cam_elbow = 2,
    read_gimbal = 3,
    read_base = 4,
    read_shoulder = 5,
    read_elbow = 6,
    read_wrist = 7,
    read_wrist_rot = 8,
    read_claw_motion = 9,
    read_claw_torque = 10
};

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
