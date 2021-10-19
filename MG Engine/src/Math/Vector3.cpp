#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "EngineMath.h"

Vector3::Vector3() : X(0), Y(0), Z(0) { }
Vector3::Vector3(const Vector3& V) : X(V.X), Y(V.Y), Z(V.Z) { }
Vector3::Vector3(Vector3&& V) noexcept : X(V.X), Y(V.Y), Z(V.Z) { }
Vector3::Vector3(float X, float Y) : X(X), Y(Y), Z(0) { }
Vector3::Vector3(float X, float Y, float Z) : X(X), Y(Y), Z(Z) { }

void Vector3::Normalize() {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return;
	float Mag = Math::Sqrt(SqrMag);
	this->X /= Mag;
	this->Y /= Mag;
	this->Z /= Mag;
}
Vector3 Vector3::Normalized() const {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return *this;
	Vector3 V;
	float Mag = Math::Sqrt(SqrMag);
	V.X /= Mag;
	V.Y /= Mag;
	V.Z /= Mag;
	return V;
}
float Vector3::Magnitude() const {
	return Math::Sqrt(X * X + Y * Y + Z * Z);
}
float Vector3::SqrMagnitude() const {
	return X * X + Y * Y + Z * Z;
}
void Vector3::ClampMagnitude(float Min, float Max) {
	float SqrMag = SqrMagnitude();

	if (SqrMag < Min * Min) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Min / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
		this->Z *= Ratio;
	} else if (SqrMag > Max * Max) {
		float Mag = Math::Sqrt(SqrMag);
		float Ratio = Max / Mag;
		this->X *= Ratio;
		this->Y *= Ratio;
		this->Z *= Ratio;
	}
}

Vector3 Vector3::operator+(const Vector3& V) const {
	return { X + V.X, Y + V.Y, Z + V.Z };
}
Vector3 Vector3::operator+(Vector3&& V) const {
	return { X + V.X, Y + V.Y, Z + V.Z };
}
Vector3 Vector3::operator-(const Vector3& V) const {
	return { X - V.X, Y - V.Y, Z - V.Z };
}
Vector3 Vector3::operator-(Vector3&& V) const {
	return { X - V.X, Y - V.Y, Z - V.Z };
}
Vector3 Vector3::operator*(float N) const {
	return { X * N, Y * N, Z * N };
}
Vector3 Vector3::operator/(float N) const {
	return { X / N, Y / N, Z / N };
}
bool Vector3::operator==(const Vector3& V) const {
	return this->X == V.X && this->Y == V.Y && this->Z == V.Z;
}
bool Vector3::operator==(Vector3&& V) const {
	return this->X == V.X && this->Y == V.Y && this->Z == V.Z;
}
bool Vector3::operator!=(const Vector3& V) const {
	return this->X != V.X || this->Y != V.Y || this->Z != V.Z;
}
bool Vector3::operator!=(Vector3&& V) const {
	return this->X != V.X || this->Y != V.Y || this->Z != V.Z;
}

Vector3::Vector3(Vector2 V) : X(V.X), Y(V.Y), Z(0) { }
Vector3::Vector3(Vector4 V) : X(V.X), Y(V.Y), Z(V.Z) { }

Vector3 Vector3::Dot(Vector3 X, Vector3 Y) {
	return { X.X * Y.X, X.Y * Y.Y, X.Z * Y.Z};
}

std::string Vector3::ToString() const {
	return "Vector3(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";
}
int Vector3::GetHashCode() const {
	return typeid(Vector3).hash_code();
}