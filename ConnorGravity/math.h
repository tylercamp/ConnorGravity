
#ifndef _ORGUI_UTIL_MATH_H_
#define _ORGUI_UTIL_MATH_H_

#include <cmath>

/* Some convenience things */

#ifndef MATH_PI
#define MATH_2_PI	6.28318530717958
#define MATH_PI		3.14159265358979
#define MATH_PI_2	1.57079632679489
#endif

#define INFINITY 1.0e+4f
#define INFINITESIMAL 1.0e-4f

inline int UpperPower (int value, int power)
{
	return powf (power, ceilf (log (value) / log (power)));
}

//	Degrees To Radians
#define dtr(x) ((x)*MATH_PI/180.0f)
//	Radians to Degrees
#define rtd(x) ((x)*180/MATH_PI)

#define xor(a,b) (((a) || (b)) && !((a) && (b)))

//	LERPs between A and B by normal N
#define lerp(a,b,n) ((a) + ((b)-(a))*n)

#define sqr(x) ((x)*(x))

#define round(x) (floor ((x)+0.5))

#ifndef max
inline float max (float a, float b)
{
	return (a>b)?a:b;
}
#endif

#ifndef min
inline float min (float a, float b)
{
	return (a<b)?a:b;
}
#endif

#define log2(x) (logf(x)/logf(2.f))

inline float wrapf (float val, float min, float max)
{
	//	Can optimize with expression literals

	if (val < min)
		return max - (min-val);
	if (val > max)
		return min + (val-max);
	return val;
}

inline float clampf (float val, float min, float max)
{
	if (val < min)
		val = min;
	if (val > max)
		val = max;
	return val;
}

#endif