#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED

#ifdef _cplusplus
extern "C"{
#endif

#define SEND_ITEM_SIZE 2
#define READ_ITEM_SIZE 2

extern const char* send_data_label[];

typedef enum {
    send_data_name_1=0,
    send_data_name_2=1,
    read_data_name_1=0,
    read_data_name_2=1
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
