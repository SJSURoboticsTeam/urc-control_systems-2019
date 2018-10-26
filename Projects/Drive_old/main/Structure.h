#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED

#ifdef _cplusplus
extern "C"{
#endif

#define SEND_ITEM_SIZE 2
#define READ_ITEM_SIZE 11

extern const char* send_data_label[];

typedef enum {
    //Motor Speed and direction 
    MS1=0,
    MS2=1,
    MS3=2,
    MS4=3,
    //Servo
    SA1=4,
    SA2=5,
    SA3=6,
    SA4=7,
    //Other
    M=8,
    DW=9,
    BRAKE=10,
    //Send Data
    Current=0,
    Sensor=1
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
