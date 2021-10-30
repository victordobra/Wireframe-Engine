#include "Quaternion.h"
#include "EngineMath.h"
#include "Vector3.h"
#include "Vector4.h"

Quaternion::Quaternion() : X(1), Y(0), Z(0), W(0) { }
Quaternion::Quaternion(const Quaternion& Q) : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }
Quaternion::Quaternion(Quaternion&& Q) noexcept : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }
Quaternion::Quaternion(float X, float Y, float Z, float W) : X(X), Y(Y), Z(Z), W(W) { }
Quaternion::Quaternion(Vector3 V, float W) : X(V.X), Y(V.Y), Z(V.Z), W(W) { }

void Quaternion::Normalize() {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return;
	float Mag = Math::Sqrt(SqrMag);
	this->X /= Mag;
	this->Y /= Mag;
	this->Z /= Mag;
	this->W /= Mag;
}
Quaternion Quaternion::Normalized() const {
	float SqrMag = SqrMagnitude();
	if (SqrMag == 1 || SqrMag == 0)
		return *this;
	Quaternion V;
	float Mag = Math::Sqrt(SqrMag);
	V.X /= Mag;
	V.Y /= Mag;
	V.Z /= Mag;
	V.W /= Mag;
	return V;
}
void Quaternion::Invert() {
	X = -X; Y = -Y; Z = -Z;
}
Quaternion Quaternion::Inverted() const {
	return { -X, -Y, -Z, W };
}
float Quaternion::Magnitude() const {
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}
float Quaternion::SqrMagnitude() const {
	return X * X + Y * Y + Z * Z + W * W;
}

Quaternion Quaternion::operator*(const Quaternion& Q) const {
	return { W * Q.W - X * Q.X - Y * Q.Y - Z * Q.Z,
			 W * Q.X + X * Q.W + Y * Q.Z - Z * Q.Y,
			 W * Q.Y - X * Q.Z + Y * Q.W + Z * Q.X,
			 W * Q.Z + X * Q.Y - Y * Q.X + Z * Q.W };
}
Quaternion Quaternion::operator*(Quaternion&& Q) const {
	return { W * Q.W - X * Q.X - Y * Q.Y - Z * Q.Z,
			 W * Q.X + X * Q.W + Y * Q.Z - Z * Q.Y,
			 W * Q.Y - X * Q.Z + Y * Q.W + Z * Q.X,
			 W * Q.Z + X * Q.Y - Y * Q.X + Z * Q.W };
}
Quaternion Quaternion::operator/(const Quaternion& Q) const {
	return operator*(Q.Inverted());
}
Quaternion Quaternion::operator/(Quaternion&& Q) const {
	return operator*(Q.Inverted());
}

Quaternion& Quaternion::operator=(const Quaternion& Q) {
	if (this == &Q)
		return *this;

	X = Q.X; Y = Q.Y; Z = Q.Z; W = Q.W;

	return *this;
}
Quaternion& Quaternion::operator=(Quaternion&& Q) noexcept {
	X = Q.X; Y = Q.Y; Z = Q.Z; W = Q.W;

	return *this;
}

bool Quaternion::operator==(const Quaternion& Q) const {
	return this->X == Q.X && this->Y == Q.Y && this->Z == Q.Z && this->W == Q.W;
}
bool Quaternion::operator==(Quaternion&& Q) const {
	return this->X == Q.X && this->Y == Q.Y && this->Z == Q.Z && this->W == Q.W;
}
bool Quaternion::operator!=(const Quaternion& Q) const {
	return this->X != Q.X || this->Y != Q.Y || this->Z != Q.Z || this->W == Q.W;
}
bool Quaternion::operator!=(Quaternion&& Q) const {
	return this->X != Q.X || this->Y != Q.Y || this->Z != Q.Z || this->W == Q.W;
}

Quaternion::Quaternion(const Vector3& V) : X(V.X), Y(V.Y), Z(V.Z), W(0) { }
Quaternion::Quaternion(Vector3&& V) : X(V.X), Y(V.Y), Z(V.Z), W(0) { }
Quaternion::Quaternion(const Vector4& V) : X(V.X), Y(V.Y), Z(V.Z), W(V.W) { }
Quaternion::Quaternion(Vector4&& V) : X(V.X), Y(V.Y), Z(V.Z), W(V.W) { }

Quaternion Quaternion::EulerAngles(float X, float Y, float Z) {
	return AroundAxis(Z, Vector3(0, 0, 1)) * AroundAxis(X, Vector3(1, 0, 0)) * AroundAxis(Y, Vector3(0, 1, 0));
}
Quaternion Quaternion::EulerAngles(Vector3 EulerAngles) {
	return AroundAxis(EulerAngles.Z, Vector3(0, 0, 1)) * AroundAxis(EulerAngles.X, Vector3(1, 0, 0)) * AroundAxis(EulerAngles.Y, Vector3(0, 1, 0));
}
Quaternion Quaternion::AroundAxis(float Angle, Vector3 Axis) {
	float SqrMag = Axis.SqrMagnitude();
	if (SqrMag == 0) 
		return { };
	if (SqrMag == 1) 
		return Quaternion(Axis * Math::Sin(Angle * Math::DegToRad), Angle * Math::Cos(Angle * Math::DegToRad));

	return Quaternion(Axis / Math::Sqrt(SqrMag) * Math::Sin(Angle * Math::DegToRad), Angle * Math::Cos(Angle * Math::DegToRad));
}


std::string Quaternion::ToString() const {
	return "Quaternion(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + "," + std::to_string(W) + ")";
}
size_t Quaternion::GetHashCode() const {
	return typeid(Quaternion).hash_code();
}