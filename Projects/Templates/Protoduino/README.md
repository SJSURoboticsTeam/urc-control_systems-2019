# README #

### Framework ###

##### General #####
* The framework make commands ONLY work in Linux. If you have a windows computer. Please install a VM of Linux using VM Workstation. If your computer is not fast enough for a VM then you will have to duel boot. Also, for VM's make sure you use a 32 bit version of Linux.

##### Hierarchy #####
* Build - The folder where all sub-systems module is placed in 
* Build/Subsystem - Contains the main file,Makefile,documentation folder, and source folder.
* Build/Subsystem/Source -subsystem function library and other ultility library . 
* Build/Subsystem/Documentation - Contain all document related to software and hardware component of the current subsystem.

##### Setting the Subsystem folder #####
1. Create a new project folder in the **Build directory** by copying the Protolobe folder with a different name.
2. Change Protolobe.c and Protolobe.h to "Your_Subsytem_Name".c and "Your_Subsystem_Name".h in the **Your Subsystem directory**.
3. Next, change the #include "Protolobe.h" to #include "Your_Subsystem_Name.h".
4. Open up the Structure.h in **Source directory** and change the enum name and send variable and read variable size as neccessary.Below is an example of how I would change the structure.h for the Arm Subsystem.

```C
#ifndef STRUCTURE_INCLUDED
#define STRUCTURE_INCLUDED
#ifdef _cplusplus
extern "C"{
#endif
#define SEND_ITEM_SIZE 2
#define READ_ITEM_SIZE 3
typedef enum {
    //section for data to be send to mission control
    current=0,
    voltage=1,
    //section for data to be read from mission control
    shoulder=0,
    elbow=1,
    claw=3
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
```
* Now, go into Structure.c and change the following char array for  :
```C
const char* send_data_label[SEND_ITEM_SIZE] = {"send_ddata_name_1","send_data_name_2"};
```
* to
```C
const char* send_data_label[SEND_ITEM_SIZE] = {"current","voltage"};
```
* Look in the the Protolobe.cpp and RTOS_tasks.cpp file,these have a example functions that show how to read and write to data in the Structure.h . Remember that only variable in struct send_data{} will be send to Mission Control and all data from mission control will be stored in struct read_data{}.

##### Sending and Receiving data #####

1. To send data to Mission Control use the following function : 
```C
**
*Convert all data in struct send_data{} to a string and send over to mission control
*param int item_size: pass in the number of how many item in send_data
**
void send_data_wifi(int item_size)
```
* to use this function, call it exactly as follow 
```C
send_data_wifi(SEND_ITEM_SIZE);
```
2. To read data from Mission Control use the following function : 
```C
**
*Convert string from Mission Control to integer value and store them in read_data
*param int item_size: pass in the number of how many item in read_data
**
void read_data_wifi(int item_size)
```
* to use this function call it exactly as follow
```C
read_data_wifi(READ_ITEM_SIZE);
```

### Unit Testing ###

##### Basics #####
* TBD
##### Suites #####
* TBD
##### Tests #####
* TBD
##### Stubs #####


### Setting Up ESP Loader ###

##### For Linux #####

1. Follow instruction from https://esp-idf.readthedocs.io/en/v2.0/linux-setup.html#step-0-rerequisites

2. Go into **Your Subsystem directory** and type "make flash"

### Make Commands ###

* `make menuconfig` - Open the config menu for the ESP32 toolchain.
* `make flash` - Flash your program onto the ESP32.
* `make monitor` - Creates all serial monitor with ESP32.

