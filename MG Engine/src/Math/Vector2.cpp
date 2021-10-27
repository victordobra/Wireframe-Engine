#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "EngineMath.h"

Vector2::Vector2() : X(0), Y(0) { }
Vector2::Vector2(const Vector2& V) : X(V.X), Y(V.Y) { }
Vector2::Vector2(Vector2&& V) noexcept : X(V.X), Y(V.Y) { }
Vector2::Vector2(float X, float Y) : X(X), Y(Y) { }

void Vector2::Normalize() {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return;
	float Mag = Math::Sqrt(SqrMag);
	this->X /= Mag;
	this->Y /= Mag;
}
Vector2 Vector2::Normalized() const {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return *this;
	Vector2 V;
	float Mag = Math::Sqrt(SqrMag);
	V.X /= Mag;
	V.Y /= Mag;
	return V;
}
float Vector2::Magnitude() const {
	return Math::Sqrt(X * X + Y * Y);
}
float Vector2::SqrMagnitude() const {
	return X * X + Y * Y;
}
void Vector2::ClampMagnitude(float Min, float Max) {
	float SqrMag = SqrMagnitude();

	if (SqrMag < Min * Min) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Min / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
	}
	else if (SqrMag > Max * Max) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Max / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
	}
}

Vector2 Vector2::operator+(const Vector2& V) const {
	return { X + V.X, Y + V.Y };
}
Vector2 Vector2::operator+(Vector2&& V) const {
	return { X + V.X, Y + V.Y };
}
Vector2 Vector2::operator-(const Vector2& V) const {
	return { X - V.X, Y - V.Y };
}
Vector2 Vector2::operator-(Vector2&& V) const {
	return { X - V.X, Y - V.Y };
}
Vector2 Vector2::operator*(float N) const {
	return { X * N, Y * N};
}
Vector2 Vector2::operator/(float N) const {
	return { X / N, Y / N };
}

Vector2& Vector2::operator=(const Vector2& V) {
	if (this == &V)
		return *this;

	X = V.X; Y = V.Y;

	return *this;
}
Vector2& Vector2::operator=(Vector2&& V) noexcept {
	X = V.X; Y = V.Y;

	return *this;
}

bool Vector2::operator==(const Vector2& V) const {
	return this->X == V.X && this->Y == V.Y;
}
bool Vector2::operator==(Vector2&& V) const {
	return this->X == V.X && this->Y == V.Y;
}
bool Vector2::operator!=(const Vector2& V) const {
	return this->X != V.X || this->Y != V.Y;
}
bool Vector2::operator!=(Vector2&& V) const {
	return this->X != V.X || this->Y != V.Y;
}

Vector2::Vector2(const Vector3& V) : X(V.X), Y(V.Y) { }
Vector2::Vector2(Vector3&& V) : X(V.X), Y(V.Y) { }
Vector2::Vector2(const Vector4& V) : X(V.X), Y(V.Y) { }
Vector2::Vector2(Vector4&& V) : X(V.X), Y(V.Y) { }

float Vector2::Dot(const Vector2& X, const Vector2& Y) {
	return X.X * Y.X + Y.X * Y.Y;
}
float Vector2::Dot(Vector2&& X, const Vector2& Y) {
	return X.X * Y.X + Y.X * Y.Y;
}
float Vector2::Dot(const Vector2& X, Vector2&& Y) {
	return X.X * Y.X + Y.X * Y.Y;
}
float Vector2::Dot(Vector2&& X, Vector2&& Y) {
	return X.X * Y.X + Y.X * Y.Y;
}

std::string Vector2::ToString() const {
	return "Vector2(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
}
int Vector2::GetHashCode() const {
	return typeid(Vector2).hash_code();
}