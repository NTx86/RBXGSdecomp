#include "util/Extents.h"
#include "util/Math.h"
#include "util/Debug.h"
#include "util/NormalId.h"

namespace RBX
{
	float Extents::areaXZ() const
	{
		Vector3 delta = this->high - this->low;
		return delta.x * delta.z;
	}

	NormalId Extents::closestFace(const Vector3& point)
	{
		NormalId faceId = NORM_X;

		// bruh
		float hy = fabs(point.y - this->high.y);
		float hz = fabs(point.z - this->high.z);
		float lx = fabs(point.x - this->low.x);
		float ly = fabs(point.y - this->low.y);
		float lz = fabs(point.z - this->low.z);

		float hx = fabs(point.x - this->high.x); 
		if (hx > hy)
		{
			faceId = NORM_Y;
		}
		else
		{
			hy = hx;
		}

		if (hz < hy)
		{
			hy = hz;
			faceId = NORM_Z;
		}

		if (lx < hy)
		{
			hy = lx;
			faceId = NORM_X_NEG;
		}

		if (ly < hy)
		{
			hy = ly;
			faceId = NORM_Y_NEG;
		}

		if (lz < hy)
		{
			faceId = NORM_Z_NEG;
		}

		return faceId;
	}

	Vector3 Extents::getCorner(int i) const
	{
		RBXAssert(i >= 0);
		RBXAssert(i <= 7);

		return Vector3(
			this->low[0 + 3 * (i / 4)],
			this->low[1 + 3 * (i / 2 % 2)],
			this->low[2 + 3 * (i % 2)]
		);
	}

	void Extents::getFaceCorners(NormalId faceId, Vector3& v0, Vector3& v1, Vector3& v2, Vector3& v3) const
	{
		switch (faceId)
		{
		case NORM_X:
			v0 = Vector3(this->high.x, this->low.y, this->low.z);
			v1 = Vector3(this->high.x, this->high.y, this->low.z);
			v2 = Vector3(this->high.x, this->high.y, this->high.z);
			v3 = Vector3(this->high.x, this->low.y, this->high.z);
			break;
		case NORM_Y:
			v0 = Vector3(this->low.x, this->high.y, this->low.z);
			v1 = Vector3(this->low.x, this->high.y, this->high.z);
			v2 = Vector3(this->high.x, this->high.y, this->high.z);
			v3 = Vector3(this->high.x, this->high.y, this->low.z);
			break;
		case NORM_Z:
			v0 = Vector3(this->low.x, this->low.y, this->high.z);
			v1 = Vector3(this->high.x, this->low.y, this->high.z);
			v2 = Vector3(this->high.x, this->high.y, this->high.z);
			v3 = Vector3(this->low.x, this->high.y, this->high.z);
			break;
		case NORM_X_NEG:
			v0 = Vector3(this->low.x, this->low.y, this->low.z);
			v1 = Vector3(this->low.x, this->low.y, this->high.z);
			v2 = Vector3(this->low.x, this->high.y, this->high.z);
			v3 = Vector3(this->high.x, this->high.y, this->low.z);
			break;
		case NORM_Y_NEG:
			v0 = Vector3(this->low.x, this->low.y, this->low.z);
			v1 = Vector3(this->high.x, this->low.y, this->low.z);
			v2 = Vector3(this->high.x, this->low.y, this->high.z);
			v3 = Vector3(this->low.x, this->low.y, this->high.z);
			break;
		case NORM_Z_NEG:
			v0 = Vector3(this->low.x, this->low.y, this->low.z);
			v1 = Vector3(this->low.x, this->high.y, this->low.z);
			v2 = Vector3(this->high.x, this->high.y, this->low.z);
			v3 = Vector3(this->high.x, this->low.y, this->low.z);
			break;
		default:
			RBXAssert(0);
		}
	}

	Vector3 Extents::faceCenter(NormalId faceId) const
	{
		Vector3 v = this->center();

		int i = faceId % 3;
		if (faceId < NORM_X_NEG)
		{
			v[i] = this->high[i];
		}
		else
		{
			v[i] = this->low[i];
		}

		return v;
	}

	bool Extents::containedByFrustum(const GCamera::Frustum& frustum) const
	{
		if (frustum.faceArray.size() > 0)
		{
			for (int i = 0; i < frustum.faceArray.size(); i++)
			{
				const G3D::Plane* plane = &frustum.faceArray[i].plane;
				for (int j = 0; j < 8; j++)
				{
					Vector3 corner = getCorner(j);
					if (!plane->halfSpaceContains(corner))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	bool Extents::contains(const Vector3& point) const
	{
		return
			point.x >= this->low.x &&
			point.y >= this->low.y &&
			point.z >= this->low.z &&
			point.x <= this->high.x &&
			point.y <= this->high.y &&
			point.z <= this->high.z;
	}

	bool Extents::fuzzyContains(const Vector3& point, float slop) const
	{
		return
			point.x >= this->low.x - slop &&
			point.y >= this->low.y - slop &&
			point.z >= this->low.z - slop &&
			point.x <= this->high.x + slop &&
			point.y <= this->high.y + slop &&
			point.z <= this->high.z + slop;
	}

	bool Extents::overlapsOrTouches(const Extents& other) const
	{
		return
			!(this->low.x > other.high.x) &&
			!(this->low.y > other.high.y) &&
			!(this->low.z > other.high.z) &&
			!(this->high.y < other.low.y) &&
			!(this->high.x < other.low.x) &&
			!(this->high.z < other.low.z);
	}

	bool Extents::separatedByMoreThan(const Extents& other, float distance) const
	{
		RBXAssert(distance > 0.0);

		Vector3 dv(distance, distance, distance);

		Extents thisExpanded(*this);
		thisExpanded.low -= dv;
		thisExpanded.high += dv;

		return !thisExpanded.overlapsOrTouches(other);
	}

	Plane Extents::getPlane(NormalId normalId) const
	{
		Vector3 point = faceCenter(normalId);
		Vector3 normal = normalIdToVector3(normalId);

		return G3D::Plane(normal, point);
	}

	void Extents::unionWith(const Extents& other)
	{
		this->low = this->low.min(other.low);
		this->high = this->high.max(other.high);
	}

	Extents Extents::toWorldSpace(const CoordinateFrame& localCoord)
	{
		Vector3 minC(Math::inf(), Math::inf(), Math::inf());
		Vector3 maxC(-Math::inf(), -Math::inf(), -Math::inf());

		for (int i = 0; i < 8; i++)
		{
			Vector3 corner = getCorner(i);
			Vector3 world = localCoord.pointToWorldSpace(corner);

			minC = minC.min(world);
			maxC = maxC.max(world);
		}

		Extents result = vv(minC, maxC);
		return result;
	}

	Extents Extents::express(const CoordinateFrame& myFrame, const CoordinateFrame& expressInFrame)
	{
		Vector3 minC(Math::inf(), Math::inf(), Math::inf());
		Vector3 maxC(-Math::inf(), -Math::inf(), -Math::inf());

		for (int i = 0; i < 8; i++)
		{
			Vector3 corner = getCorner(i);
			Vector3 v1 = myFrame.pointToWorldSpace(corner) - expressInFrame.translation;
			Vector3 inOther = v1 * expressInFrame.rotation;

			minC = minC.min(inOther);
			maxC = maxC.max(inOther);
		}

		Extents result = vv(minC, maxC);
		return result;
	}

	Extents Extents::vv(const Vector3& v0, const Vector3& v1)
	{
		Extents result;

		result.low = v0.min(v1);
		result.high = v0.max(v1);

		return result;
	}

	const Extents& Extents::zero()
	{
		static Extents e(Vector3::zero(), Vector3::zero());
		return e;
	}

	const Extents& Extents::negativeInfiniteExtents()
	{
		// Extents default constructor sets high and low to inf
		static Extents e;
		return e;
	}
}
