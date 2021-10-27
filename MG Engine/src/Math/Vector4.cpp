#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "EngineMath.h"

Vector4::Vector4() : X(0), Y(0), Z(0), W(0) { }
Vector4::Vector4(const Vector4& V) : X(V.X), Y(V.Y), Z(V.Z), W(V.W) { }
Vector4::Vector4(Vector4&& V) noexcept : X(V.X), Y(V.Y), Z(V.Z), W(V.W) { }
Vector4::Vector4(float X, float Y, float Z, float W) : X(X), Y(Y), Z(Z), W(W) { }

void Vector4::Normalize() {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return;
	float Mag = Math::Sqrt(SqrMag);
	this->X /= Mag;
	this->Y /= Mag;
	this->Z /= Mag;
	this->W /= Mag;
}
Vector4 Vector4::Normalized() const {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return *this;
	Vector4 V;
	float Mag = Math::Sqrt(SqrMag);
	V.X /= Mag;
	V.Y /= Mag;
	V.Z /= Mag;
	V.W /= Mag;
	return V;
}
float Vector4::Magnitude() const {
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}
float Vector4::SqrMagnitude() const {
	return X * X + Y * Y + Z * Z + W * W;
}
void Vector4::ClampMagnitude(float Min, float Max) {
	float SqrMag = SqrMagnitude();

	if (SqrMag < Min * Min) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Min / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
		this->Z *= Ratio;
		this->W *= Ratio;
	}
	else if (SqrMag > Max * Max) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Max / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
		this->Z *= Ratio;
		this->W *= Ratio;
	}
}

Vector4 Vector4::operator+(const Vector4& V) const {
	return { X + V.X, Y + V.Y, Z + V.Z, W + V.W };
}
Vector4 Vector4::operator+(Vector4&& V) const {
	return { X + V.X, Y + V.Y, Z + V.Z, W + V.W };
}
Vector4 Vector4::operator-(const Vector4& V) const {
	return { X - V.X, Y - V.Y, Z - V.Z, W - V.W };
}
Vector4 Vector4::operator-(Vector4&& V) const {
	return { X - V.X, Y - V.Y, Z - V.Z, W - V.W };
}
Vector4 Vector4::operator*(float N) const {
	return { X * N, Y * N, Z * N, W * N };
}
Vector4 Vector4::operator/(float N) const {
	return { X / N, Y / N, Z / N, W / N };
}

Vector4& Vector4::operator=(const Vector4& V) {
	if (this == &V)
		return *this;

	X = V.X; Y = V.Y; Z = V.Z; W = V.W;

	return *this;
}
Vector4& Vector4::operator=(Vector4&& V) noexcept {
	X = V.X; Y = V.Y; Z = V.Z; W = V.W;

	return *this;
}

bool Vector4::operator==(const Vector4& V) const {
	return this->X == V.X && this->Y == V.Y && this->Z == V.Z && this->W == V.W;
}
bool Vector4::operator==(Vector4&& V) const {
	return this->X == V.X && this->Y == V.Y && this->Z == V.Z && this->W == V.W;
}
bool Vector4::operator!=(const Vector4& V) const {
	return this->X != V.X || this->Y != V.Y || this->Z != V.Z || this->W == V.W;
}
bool Vector4::operator!=(Vector4&& V) const {
	return this->X != V.X || this->Y != V.Y || this->Z != V.Z || this->W == V.W;
}

Vector4::Vector4(const Vector2& V) : X(V.X), Y(V.Y), Z(0), W(0) { }
Vector4::Vector4(Vector2&& V) : X(V.X), Y(V.Y), Z(0), W(0) { }
Vector4::Vector4(const Vector3& V) : X(V.X), Y(V.Y), Z(V.Z), W(0) { }
Vector4::Vector4(Vector3&& V) : X(V.X), Y(V.Y), Z(V.Z), W(0) { }
Vector4::Vector4(const Quaternion& Q) : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }
Vector4::Vector4(Quaternion&& Q) : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }

float Vector4::Dot(const Vector4& X, const Vector4& Y) {
	return X.X * Y.X + X.Y * Y.Y + X.Z * Y.Z + X.W * Y.W;
}
float Vector4::Dot(Vector4&& X, const Vector4& Y) {
	return X.X * Y.X + X.Y * Y.Y + X.Z * Y.Z + X.W * Y.W;
}
float Vector4::Dot(const Vector4& X, Vector4&& Y) {
	return X.X * Y.X + X.Y * Y.Y + X.Z * Y.Z + X.W * Y.W;
}
float Vector4::Dot(Vector4&& X, Vector4&& Y) {
	return X.X * Y.X + X.Y * Y.Y + X.Z * Y.Z + X.W * Y.W;
}

std::string Vector4::ToString() const {
	return "Vector4(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + "," + std::to_string(W) + ")";
}
int Vector4::GetHashCode() const {
	return typeid(Vector4).hash_code();
}