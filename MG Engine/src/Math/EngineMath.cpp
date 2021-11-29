#include "EngineMath.h"
#include <math.h>

float Math::Sin(float X) { return sinf(X); }
float Math::Cos(float X) { return cosf(X); }
float Math::Tan(float X) { return tanf(X); }
float Math::Asin(float X) { return asinf(X); }
float Math::Acos(float X) { return acosf(X); }
float Math::Atan(float X) { return atanf(X); }
float Math::Atan2(float X, float Y) { return atan2f(X, Y); }

float Math::Sqrt(float X) { return sqrtf(X); }
float Math::Lerp(float X, float Y, float T) { return X + (Y - X) * T; }
float Math::Lerp(int X, int Y, float T) { return X + (int)((Y - X) * T); }
float Math::Slerp(float X, float Y, float T) { return X + (Y - X) / (1 + 1 / powf(Math::e, T)); }
float Math::Slerp(int X, int Y, float T) { return X + (int)((Y - X) / (1 + 1 / powf(Math::e, T))); }