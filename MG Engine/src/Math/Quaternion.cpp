#include "Quaternion.h"
#include "EngineMath.h"

Quaternion::Quaternion() : X(1), Y(0), Z(0), W(0) { }
Quaternion::Quaternion(const Quaternion& Q) : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }
Quaternion::Quaternion(Quaternion&& Q) noexcept : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) { }
Quaternion::Quaternion(float X, float Y, float Z, float W) : X(X), Y(Y), Z(Z), W(W) { }

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
float Quaternion::Magnitude() const {
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}
float Quaternion::SqrMagnitude() const {
	return X * X + Y * Y + Z * Z + W * W;
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
Quaternion Quaternion::operator*(const Quaternion& Q) const {
	
}

std::string Quaternion::ToString() const {
	return "Quaternion(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + "," + std::to_string(W) + ")";
}
int Quaternion::GetHashCode() const {
	return typeid(Quaternion).hash_code();
}