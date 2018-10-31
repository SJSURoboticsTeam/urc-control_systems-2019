#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////

int degreesToDuty(unsigned int degrees)
{
    int duty = -1;
    if(degrees <= 360)
    {
        double degreePercentage;

        degreePercentage = degrees / 360;
        duty = static_cast<int>(degreePercentage * 1024);

    }
    return duty;
}

////////////////////////////////////////////////////////////////////////////////
//                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
