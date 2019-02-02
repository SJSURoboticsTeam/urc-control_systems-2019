////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "freertos/semphr.h"
#include "Arduino.h"



#define EEPROM_SIZE 0x64

#define BEGINING_ADDR 0x00

extern SemaphoreHandle_t xButtonInterruptSemaphore;

extern portMUX_TYPE mux;

#endif