# SJSU ROBOTICS - CONTROL SYSTEMS 2019 # 
[![Build Status](https://travis-ci.com/SJSURobotics2019/controlsystems2019.svg?branch=master)](https://travis-ci.com/SJSURobotics2019/controlsystems2019)
### Framework ###

##### General #####
* The framework make commands ONLY work in Linux. If you have a windows computer. Please install a VM of Linux using VM Workstation. If your computer is not fast enough for a VM then you will have to dual boot. Also, for VM's, make sure you use a 32 bit version of Linux.
* **DO NOT USE THE CLONE BUTTON AT THE TOP OF THIS PAGE.**     
Instead use the command `git clone --recursive https://github.com/SJSURobotics2019/controlsystems2019.git`

##### Hierarchy #####
* Projects - The folder where all sub-systems module is placed in.
* Projects/Subsystem - Contains the main file,Makefile,documentation folder, and source folder.
* Projects/Subsystem/Source -subsystem function library and other ultility library. 
* Projects/Subsystem/Documentation - Contain all document related to software and hardware component of the current subsystem.
* Projects/Subsystem/Test - Contains the unit testing files for the current subsystem.

##### Setting the Subsystem folder #####
1. Create a new project folder in the **Projects directory** by copying the Protolobe folder with a different name.
2. Place your methods in Source.c and Source.h, and your tasks in RTOS_Tasks.cpp and RTOS_Tasks.h.
3. Open up the Structure.h in **Source directory** and change the enum name and send variable and read variable size as neccessary. Below is an example of how I would change the structure.h for the Arm Subsystem.

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
const char* send_data_label[] = {"send_data_name_1","send_data_name_2"};
```
* to (if the arrays do not compile correctly, put SEND_ITEM_SIZE in the brackets)
```C
const char* send_data_label[] = {"current","voltage"};
```
* to write voltage value do the following 
```C
send_data.data[voltage]=20;
```
* to print out shoulder value do the following
```C
printf("%i",read_data.data[shoulder]);
```
* Look in the the Protolobe.cpp and RTOS_tasks.cpp file,these have a example functions that show how to read and write to data in the Structure.h . Remember that only variable in struct send_data{} will be send to Mission Control and all data from mission control will be stored in struct read_data{}.

##### Setting Up WI-FI Access Point Name and IP Address #####

1. To set your ESP's access point name, use the following function :
```C
set_wifi_ap("<name>", <name_size>);
```
2. To set your ESP's IP address, use the following function:
```C
set_static_ip("<IP_address>");
```
3. The AP (access point) name and IP address must be set during initialization. i.e. in your subsystem's init function.
##### Sending and Receiving data #####

1. To send data to Mission Control use the following function : 
```C
void send_data_wifi(int item_size)
```
* Convert all data in struct send_data{} to a string and send over to mission control
* param int item_size: pass in the number of how many item in send_data
* to use this function, call it exactly as follow 
```C
send_data_wifi(SEND_ITEM_SIZE);
```
2. To read data from Mission Control use the following function : 
```C
void read_data_wifi(int item_size)
```
* Convert string from Mission Control to integer value and store them in read_data
* param int item_size: pass in the number of how many item in read_data
* to use this function call it exactly as follow
```C
read_data_wifi(READ_ITEM_SIZE);
```

### Unit Testing ###

##### Basics #####
* We use Catch2 for unit testing C++.
    * https://github.com/catchorg/Catch2/tree/master/docs
    * https://github.com/meekrosoft/fff
* Writing unit tests are easy with catch2.
    * On your fork, write your test cases in Testing/unitTests.cpp
    * Write test cases; follow Testing/catch2test.cpp as an example.
    * To test:        
        ```
        cd Testing
        g++ -std=c++11 unitTests.cpp
        ./a.out
        ```
        This will run all of your unit tests.

##### Suites #####
* TBD

##### Tests #####
* TBD

##### Stubs #####
* Stubs are used to mock functions which manipulate or utilize hardware such as gpio.  Stubs that are used generally throughout all subsystems are located in the following directory: "controlsystems2018/Testing/stubs/"
* If there are functions that you require to be mocked but is not in the general stubs folder, you must create it inside the testing subfolder in your subsystem's project directory.
* Steps to mock a function
1. Create the header file in which the function is located inside of the test directory in your subsystem.
2. Declare the function, and any required structures, in the same way as in the original header file.
3. Inside your tests.cpp, mock void functions as followed: "FAKE_VOID_FUNC(function_name, parameters, ...);"
   To mock functions which return a value: "FAKE_VALUE_FUNC(return_type, function_name, parameters, ...);"

### Setting Up ESP Loader ###

##### For Linux #####
1. Install the required package to compile with esp-idf.  
Terminal command: `sudo apt-get install git wget make libncurses-dev flex bison gperf python python-serial`     
**Note:** one or more of these packages may fail to install. Should that happen, perform a `sudo apt-get update` and try again. 

2. Set your PATH variables in .profile to add paths to the esp-idf and the xtensa toolchain. The .profile file is hidden in the Home directory and can be found by pressing ctrl+h when in Home. In the terminal you can find the file by using the following commands:     
`cd ~`     
`ls -a`     
**The lines to add are:**  
`export PATH=$PATH:(directory path to controlsystems2019)/controlsystems2019/xtensa-esp32-elf/bin`    
`export IDF_PATH=(directory path to controlsystems2019)/controlsystems2019/esp-idf`  
**Example lines:**  
`export PATH=$PATH:$HOME/Documents/2018-2019/controlsystems2019/xtensa-esp32-elf/bin`  
`export IDF_PATH=~/Documents/2018-2019/controlsystems2018/esp-idf`     
**Note:** you may wish to change the access permissions to your COM ports at this point as you will need to do so in order to program the esp32 boards. To do this, add the command `sudo chmod 666 /dev/tty*` after the PATH commands in .profile.
Once you update the paths, enter the command `source ~/.profile` for the changes to take effect. You must enter this command for each terminal or bash session.

3. Confirm you correctly set your PATH variables by entering either the command `echo $PATH` or `printenv $PATH`. In both cases, you should see a long list of file paths with the path you just created at the end of it. Do the same commands for IDF_PATH and you should see only the path that you entered above.

4. Test that you have installed and set up the esp-idf environment correctly by configuring and flashing hello_world to your ESP32. navigate to the Projects/Examples/hello_world folder and enter the command `make menuconfig PROJECT_NAME=hello-world`. This should open the ESP32 toolchain configuration menu. Navigate to Serial Flasher config -> Default Serial Port and change this value to the usb port that your ESP32 is connected to. 
   If you are using windows, these are your COM ports, so you will need to determine which of these your ESP32 is connected to and use the linux naming configuration. For example, if your ESP32 is connected to COM4, you would enter /dev/ttyS4. 
   Once done, save and exit the configuration menu. Run the command `make app-flash PROJECT_NAME=hello-world` to build and program your ESP32. When the program has finished building and flashing, run the command `make monitor` to see if you are recieving from your board. Press ctrl+] to exit.
   
### Using Arduino Libraries ###
To use Arduino libraries, copy the contents of Templates/Protoduino into a new project including the "components" folder. This folder contains the Arduino libraries and framework. Run `make menuconfig` and navigate to the Arduino options. Uncheck "Autostart Arduino setup and loop on boot" if it is not already. Navigate to Serial Flasher config and change the flash size from 2MB to 4 MB. Configure other settings as you normally would. Close the menuconfig and open main->main.cpp. Make sure that the Arduino.h header and the following lines of code are included in your project:     
```C
#include "Arduino.h"

extern "C" void app_main()
{
    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

}

```
Add aditional Arduino libraries to your #includes as necessary.

### Make Commands ###

* `make menuconfig PROJECT_NAME=<Subsystem name>` - Open the config menu for the ESP32 toolchain.
* `make app-flash PROJECT_NAME=<Subsystem folder name>` - Flash your program onto the ESP32.
* `make app PROJECT_NAME=<Subsystem folder name>` - Build your program.
* `make monitor PROJECT_NAME=<Subsystem folder name>` - Creates all serial monitor with ESP32.

### Make Process ###
1. After coding your project, perform a `make menuconfig PROJECT_NAME=<Subsystem name>` to access the SDK configuration menu.
2. Navigate to and select Serial Flash Config and make sure the serial port setting is the port that the ESP32 is connected to.
3. Save the settings and exit.
4. Perform a `make app-flash PROJECT_NAME=<Subsystem folder name>` to build and upload your program to the ESP32.
	* if you only want to build the program, then perform `make app PROJECT_NAME=<Subsystem folder name>` 

### Troubleshooting ###

##### Corrupted Bootloader #####
If at any point in time your ESP's "make monitor" starts printing out resets or unknown characters, that means the bootloader on the ESP has been corrupted.  To repair this, use the official version of the esp-idf and "make flash" the hello world program shown in the tutorial.  Once the program has been flashed, you can start flashing your projects again.

##### esp_idf is empty #####
You likely pressed the "clone" button at the top of this page and copied the command to your terminal. This command does not include the `--recursive` option. To fix this, navigate to /controlsystems2019 and run `git submodule update --init --recursive`

##### "No such file or directory" and "No rule to make target" during menuconfig #####
See above. Otherwise, check to make sure you have entered your paths correctly.

##### "Input/output error: '/dev/tty<PORT NUMBER>'" #####
The project is configured to the wrong USB port. Run make menuconfig again and change the assigned port to the correct one.

##### "xtensa-esp32-elf-gcc: error trying to exec 'cc1': execvp: No such file or directory" #####
The xtensa-esp32-elf toolchain did not download and install properly. Follow the procedure as described at https://docs.espressif.com/projects/esp-idf/en/stable/get-started/linux-setup.html but install the toolchain into the /controlsystems2019 directory.

##### Cannot connect to esp32, flashing fails #####
You must press down on the button labeled "IO0" while the esp32 is connecting. You may need to press it a few times or press and hold in order for it to connect.
