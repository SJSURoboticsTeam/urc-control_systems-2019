#include "utilities.h"

inline uint16_t constrain(uint16_t in, uint16_t max, uint16_t min)
{
	uint16_t temp = in;
	temp = (temp > max) ? max : temp;
	temp = (temp < min) ? min : temp;
	// if(temp > max)
	// {
	// 	temp = max;
	// }
	// if(temp < min)
	// {
	// 	temp = min;
	// }
	return temp;
}