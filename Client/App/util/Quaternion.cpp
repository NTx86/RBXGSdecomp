#include "util/Quaternion.h"

namespace RBX
{
	Quaternion::Quaternion(const G3D::Matrix3& rot)
	{
		float v2 = rot[0][0] + rot[1][1] + rot[2][2];
		if (v2 > 0)
		{
			float v3 = sqrt(v2 + 1);
			this->w = v3 * 0.5;
			float v4 = 0.5 / v3;
			this->x = (rot[2][1] - rot[1][2]) * v4;
			this->y = (rot[0][2] - rot[2][0]) * v4;
			this->z = (rot[1][0] - rot[0][1]) * v4;
		}
		else
		{
			if (rot[0][0] > rot[1][1] && rot[0][0] > rot[2][2])
			{
				float v5 = -sqrt(rot[0][0] + 1.0 - rot[1][1] - rot[2][2]);
				this->x = v5 * 0.5;
				float v6 = 0.5 / v5;
				this->y = (rot[1][0] + rot[0][1]) * v6;
				this->z = (rot[2][0] + rot[0][2]) * v6;
				this->w = v6 * (rot[2][1] - rot[1][2]);
			}
			else
			{
				if (rot[1][1] > rot[2][2])
				{
					float v7 = -sqrt(rot[1][1] + 1.0 - rot[0][0] - rot[2][2]);
					this->y = v7 * 0.5;
					float v8 = 0.5 / v7;
					this->x = (rot[1][0] + rot[0][1]) * v8;
					this->z = (rot[2][1] + rot[1][2]) * v8;
					float v9 = rot[0][2] - rot[2][0];
					this->w = v8 * v9;
				}
				else
				{
					float v10 = -sqrt(rot[2][2] + 1.0 - rot[0][0] - rot[1][1]);
					this->z = v10 * 0.5;
					float v8 = 0.5 / v10;
					this->x = (rot[2][0] + rot[0][2]) * v8;
					this->y = (rot[2][1] + rot[1][2]) * v8;
					float v9 = rot[1][0] - rot[0][1];
					this->w = v8 * v9;
				}
			}
		}
	}

	Quaternion& Quaternion::operator=(const Quaternion& other)
	{
		this->w = other.w;
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const
	{
		Quaternion result;

		float w = this->w;
		float v4 = other.w;
		float v11 = this->z * v4;
		float v8 = this->w * other.x;
		float v9 = w * other.y;
		float v10 = other.z * w;
		float v6 = this->z * other.x - other.z * this->x + v4 * this->y + v9;
		float v7 = this->x * other.y - other.x * this->y + v10 + v11;
		result.x = other.z * this->y - this->z * other.y + this->x * v4 + v8;
		result.y = v6;
		result.z = v7;
		result.w = w * v4 - (other.x * this->x + other.z * this->z + other.y * this->y);
		return result;
	}

	float& Quaternion::operator[](int i) const
	{
		return ((float*)this)[i];
	}

	void Quaternion::normalize()
	{
		float sqY = square(this->y);
		float v1 = sqY + square(this->x) + square(this->z) + square(this->w);
		float v3 = G3D::rsqrt(v1);
		this->x = this->x * v3;
		this->y = this->y * v3;
		this->z = this->z * v3;
		this->w = this->w * v3;
	}

	void Quaternion::toRotationMatrix(G3D::Matrix3& rot) const
	{
		Quaternion QuatMul = *this * 2.0f;

		float v5 = x * QuatMul.x;
		float v6 = x * QuatMul.y; //gets loaded to non-float stack
		float v7 = x * QuatMul.z;

		float v8 = w * QuatMul.x;
		float v9 = w * QuatMul.y;
		float zz = w * QuatMul.z; //this should be loaded to non-float stack

		float v10 = y * QuatMul.y;
		float v15 = y * QuatMul.z;
		float v12 = z * QuatMul.z;

		rot[0][0] = 1.0 - (v12 + v10);
		rot[0][1] = v6 - zz; //the stack value gets loaded here
		rot[0][2] = v9 + v7;

		rot[1][0] = v9 + zz;
		rot[1][1] = 1.0 - (v12 + v5);
		rot[1][2] = v15 - v8;

		rot[2][0] = v7 - v9;
		rot[2][1] = v8 + v15;
		rot[2][2] = 1.0 - (v5 + v10);
	}

	float Quaternion::maxComponent() const
	{
		Quaternion qAbs = Quaternion(fabs(w), fabs(z), fabs(y), fabs(x));
		return std::max(std::max(qAbs.w, qAbs.z), std::max(qAbs.y, qAbs.x));
	}
}