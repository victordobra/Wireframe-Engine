#pragma once

namespace mge {
	constexpr float e = 2.71828f;
	constexpr float pi = 3.14159f;
	constexpr float radToDeg = 57.29579f;
	constexpr float degToRad = 0.01745f;

	float Sin(float x);
	float Cos(float x);
	float Tan(float x);
	float Asin(float x);
	float Acos(float x);
	float Atan(float x);
	float Atan2(float x, float y);

	float Sqrt(float x);
	float Lerp(float x, float y, float t);
	int Lerp(int x, int y, float t);
	float Slerp(float x, float y, float t);
	int Slerp(int x, int y, float t);
}