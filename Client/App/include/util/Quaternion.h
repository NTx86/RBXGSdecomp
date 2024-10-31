#pragma once
#include <G3DAll.h>

namespace RBX {
	class Quaternion
	{
		public:
			float x;
			float y;
			float z;
			float w;
			Quaternion(const G3D::Matrix3& rot);
			Quaternion();
			Quaternion(const G3D::Vector3&, float);
			Quaternion(float, float, float, float);
			Quaternion& operator=(const Quaternion& other);
			G3D::Vector3& imag();
			const G3D::Vector3& imag() const;
			void toRotationMatrix(G3D::Matrix3& rot) const;
			float dot(const Quaternion&) const;
			float magnitude() const;
			float maxComponent() const;
			float& operator[](int) const;
			operator float *();
			operator const float *() const;
			Quaternion operator*(float) const;
			Quaternion operator*(const Quaternion& other) const;
			Quaternion operator+(const Quaternion& other) const;
			Quaternion operator-(const Quaternion& other) const;
			Quaternion& operator*=(float);
			Quaternion& operator+=(const Quaternion&);
			void normalize();
	};
}