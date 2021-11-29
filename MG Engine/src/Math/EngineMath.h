#pragma once

namespace Math {
	const float e = 2.71828f;
	const float Pi = 3.14159f;
	const float RadToDeg = 57.29579f;
	const float DegToRad = 0.01745f;

	float Sin(float X);
	float Cos(float X);
	float Tan(float X);
	float Asin(float X);
	float Acos(float X);
	float Atan(float X);
	float Atan2(float X, float Y);

	float Sqrt(float X);
	float Lerp(float X, float Y, float T);
	float Lerp(int X, int Y, float T);
	float Slerp(float X, float Y, float T);
	float Slerp(int X, int Y, float T);
}