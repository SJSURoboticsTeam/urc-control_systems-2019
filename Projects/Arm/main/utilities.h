#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>
#include <stdio.h>
inline void constrain_uint(uint16_t * in, uint16_t min, uint16_t max)
{
	*in = (*in > max) ? max : *in;
	*in = (*in < min) ? min : *in;
	// if(in > max)
	// {
	// 	in = max;
	// }
	// if(in < min)
	// {
	// 	in = min;
	// }
}

inline void constrain_double(double * in, double min, double max)
{
	*in = (*in > max) ? max : *in;
	*in = (*in < min) ? min : *in;

	// if(in > max)
	// {
	// 	in = max;
	// }
	// if(in < min)
	// {
	// 	in = min;
	// }
}

inline int16_t EMA_uint(uint16_t in, uint16_t out, uint8_t alpha)
{
	return ( (in * alpha) / 1000 + (out * (1000 - alpha )) / 1000 );
}

inline double EMA_double(double in, double out, double alpha)
{
	return ( (in * alpha) + (out * (1 - alpha )));
}
#endif
