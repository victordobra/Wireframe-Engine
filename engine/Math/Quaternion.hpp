#pragma once

#include "Core.hpp"
#include <string.h>
#include <math.h>

namespace wfe {
	struct Quaternion {
		static Quaternion Identity() { return { 0.f, 0.f, 0.f, 1.f }; }
		
		union {
			float32_t elements[4]{ 0.f, 0.f, 0.f, 1.f };

			struct {
				union {
					float32_t x, r, s;
				};
				union {
					float32_t y, g, t;
				};
				union {
					float32_t z, b, p;
				};
				union {
					float32_t w, a, q;
				};
			};
		};

		Quaternion() = default;
		Quaternion(const Quaternion&) = default;
		Quaternion(Quaternion&&) noexcept = default;
		Quaternion(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) { }

		Quaternion& operator=(const Quaternion&) = default;
		Quaternion& operator=(Quaternion&&) = default;

		bool8_t operator==(const Quaternion& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool8_t operator==(Quaternion&& other)      const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool8_t operator!=(const Quaternion& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		bool8_t operator!=(Quaternion&& other)      const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		Quaternion operator*(const Quaternion& other) const {
			return {  x * other.w + y * other.z - z * other.y + w * other.x, 
			         -x * other.z + y * other.w + z * other.x + w * other.y,
			          x * other.y - y * other.x + z * other.w + w * other.z,
			         -x * other.x - y * other.y - z * other.z + w * other.w };
		}
		Quaternion operator*(Quaternion&& other) const {
			return {  x * other.w + y * other.z - z * other.y + w * other.x,
					 -x * other.z + y * other.w + z * other.x + w * other.y,
					  x * other.y - y * other.x + z * other.w + w * other.z,
					 -x * other.x - y * other.y - z * other.z + w * other.w };
		}

		Quaternion& operator*=(const Quaternion& other) {
			float32_t x = this->x, y = this->y, z = this->z, w = this->w;

			this->x = x * other.w + y * other.z - z * other.y + w * other.x;
			this->y = -x * other.z + y * other.w + z * other.x + w * other.y;
			this->z = x * other.y - y * other.x + z * other.w + w * other.z;
			this->w = -x * other.x - y * other.y - z * other.z + w * other.w;

			return *this;
		}
		Quaternion& operator*=(Quaternion&& other) noexcept {
			float32_t x = this->x, y = this->y, z = this->z, w = this->w;

			this->x =  x * other.w + y * other.z - z * other.y + w * other.x;
			this->y = -x * other.z + y * other.w + z * other.x + w * other.y;
			this->z =  x * other.y - y * other.x + z * other.w + w * other.z;
			this->w = -x * other.x - y * other.y - z * other.z + w * other.w;
			
			return *this;
		}

		/// @brief Returns the length of the quaternion.
		float32_t Magnitude() const { 
			return sqrtf(x * x + y * y + z * z + w * w); 
		}
		/// @brief Returns the square of the length of the quaternion.
		float32_t SqrMagnitude() const { 
			return x * x + y * y + z * z + w * w; 
		}
		/// @brief Returns the inverse of the magnitude.
		float32_t InvMagnitude() const {
			float32_t sqrMag = SqrMagnitude();
			if (sqrMag == 0)
				return 0;

			uint32_t i;
			float32_t halfSqrMag = sqrMag * .5f, invMag = sqrMag;
			const float32_t threeHalves = 1.5f;

			memcpy(&i, &invMag, sizeof(uint32_t));
			i = 0x5f3759df - (i >> 1);
			memcpy(&invMag, &i, sizeof(uint32_t));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));

			return invMag;
		}
		/// @brief Reduces the quaternion's length to 1.
		Quaternion& Normalize() {
			float32_t invMag = InvMagnitude();

			x *= invMag;
			y *= invMag;
			z *= invMag;
			w *= invMag;

			return *this;
		}
		/// @brief Returns a version of the quaternion with the length 1.
		Quaternion Normalized() const {
			float32_t invMag = InvMagnitude();

			Quaternion newQuat{ x * invMag, y * invMag, z * invMag, w * invMag };
			return newQuat;
		}
		/// @brief Inverts the quaternion
		Quaternion& Invert() { 
			x = -x; y = -y; z = -z; return *this; 
		}
		/// @brief Returns the inverted version of the quaternion
		Quaternion Inverted() const { 
			return { -x, -y, -z, w }; 
		};
		/// @brief Returns the dot product between two quaternions
		/// @param other The other quaternion
		float32_t Dot(const Quaternion& other) const { 
			return x * other.x + y * other.y + z * other.z + w * other.w; 
		}
		/// @brief Returns the dot product between two quaternions
		/// @param other The other quaternion
		float32_t Dot(Quaternion&& other) const { 
			return x * other.x + y * other.y + z * other.z + w * other.w; 
		}
		/// @brief Converts the quaternion to Euler angles.
		Vector3 ToEulerAngles() const {
			Vector3 euler;

			// Generate the quaternion's rotation matrix
			float32_t rows[3][3];
			rows[0][0]  = 1.f - 2.f * y * y - 2.f * z * z;
			rows[0][1]  =       2.f * x * y - 2.f * z * w;
			rows[0][2]  =       2.f * x * z + 2.f * y * w;

			rows[1][0]    =       2.f * x * y + 2.f * z * w;
			rows[1][1]    = 1.f - 2.f * x * x - 2.f * z * z;
			rows[1][2]    =       2.f * y * z - 2.f * x * w;

			rows[2][0]    =       2.f * x * z - 2.f * y * w;
			rows[2][1]    =       2.f * y * z + 2.f * x * w;
			rows[2][2]   = 1.f - 2.f * x * x - 2.f * y * y;

			// From https://github.com/godotengine/godot/blob/master/core/math/basis.cpp
			if(rows[1][2] < 1) {
				if(rows[1][2] > -1) {
					// Check whether this is a pure X rotation
					if (rows[1][0] == 0 && rows[0][1] == 0 && rows[0][2] == 0 && rows[2][0] == 0 && rows[0][0] == 1) {
						euler.x = atan2(-rows[1][2], rows[1][1]);
						euler.y = 0;
						euler.z = 0;
					} else {
						euler.x = asin(-rows[1][2]);
						euler.y = atan2(rows[0][2], rows[2][2]);
						euler.z = atan2(rows[1][0], rows[1][1]);
					}
				} else {
					euler.x = M_PI_2;
					euler.y = atan2(rows[0][1], rows[0][0]);
					euler.z = 0;
				}
			} else {
				euler.x = -M_PI_2;
				euler.y = -atan2(rows[0][1], rows[0][0]);
				euler.z = 0;
			}

			// Convert every angle to degrees
			euler.x *= 180.f / M_PI;
			euler.y *= 180.f / M_PI;
			euler.z *= 180.f / M_PI;

			return euler;
		}

		/// @brief Returns a quaternion that represents the rotation around an axis.
		/// @param axis The axis to rotate around.
		/// @param angle The angle of rotation.
		/// @param normalize (optional) Whether to normalize the quaternion or not.
		static Quaternion AroundAxis(Vector3 axis, float32_t angle, bool8_t normalize = true) {
			const float32_t halfAngle = angle * 0.5f;
			const float32_t sin = sinf(halfAngle), cos = cosf(halfAngle);

			Quaternion outQuaternion{sin * axis.x, sin * axis.y, sin * axis.z, cos};

			if (normalize)
				outQuaternion.Normalize();

			return outQuaternion;
		}
		/// @brief Returns a quaternion that represents euler angle rotation.
		/// @param eulerAngles The euler angles in radians.
		static Quaternion EulerAngles(Vector3 eulerAngles) {
			return Quaternion(AroundAxis({ 0.f, 0.f, 1.f }, eulerAngles.z, false) * AroundAxis({ 1.f, 0.f, 0.f }, eulerAngles.x, false) * AroundAxis({ 0.f, 1.f, 0.f }, eulerAngles.y, false)).Normalized();
		}

		~Quaternion() = default;
	};
}