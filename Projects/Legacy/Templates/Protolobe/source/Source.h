
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
extern "C" {
	#ifndef DEBUG
		#include "Wifi.h"
	#endif
	#ifdef DEBUG
		#include "stubs/Wifi.h"
	#endif
	#include "Structure.h"
}


#ifndef PROTOLOBE_H_
#define PROTOLOBE_H_

#ifdef _cplusplus
extern "C"{
#endif

extern "C" void Protolobe_Init();

double convert(double current, uint16_t time);


#ifdef _cplusplus
}
#endif

#endif 
