#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"

#include "Wire.h"

////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////

double readPitch()
{
    int16_t xVal, yVal, zVal;
    double pitch;
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);

    xVal = Wire.read() << 8 | Wire.read();
    yVal = Wire.read() << 8 | Wire.read();
    zVal = Wire.read() << 8 | Wire.read();

    pitch = atan2(sqrt(xVal*xVal + yVal*yVal), zVal);
    pitch *= (180 / 3.14);

    if(xVal < 0) pitch = 0 - pitch;

    printf("%i\n",xVal);
    printf("%i\n",yVal);
    printf("%i\n",zVal);

    return pitch;
}


int32_t calcDutyOffset(int8_t targetAngle, double currentAngle )
{
    double angleDifference = currentAngle - targetAngle;
    uint32_t changeOfDuty;
    uint32_t dutyCycleRange = maxDuty - minDuty;

    changeOfDuty = ( (angleDifference / 180) * dutyCycleRange);

    return changeOfDuty;

}

////////////////////////////////////////////////////////////////////////////////
//                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
