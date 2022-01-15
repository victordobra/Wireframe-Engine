#include "EngineMath.h"
#include <math.h>

namespace mge {
	float Sin(float x) { return sinf(x); }
	float Cos(float x) { return cosf(x); }
	float Tan(float x) { return tanf(x); }
	float Asin(float x) { return asinf(x); }
	float Acos(float x) { return acosf(x); }
	float Atan(float x) { return atanf(x); }
	float Atan2(float x, float y) { return atan2f(x, y); }

	float Sqrt(float x) { return sqrtf(x); }
	float Lerp(float x, float y, float t) { return x + (y - x) * t; }
	int Lerp(int x, int y, float t) { return x + (int)((y - x) * t); }
	float Slerp(float x, float y, float t) { return x + (y - x) / (1 + 1 / powf(e, t)); }
	int Slerp(int x, int y, float t) { return x + (int)((y - x) / (1 + 1 / powf(e, t))); }
}