#pragma once
#include <G3DAll.h>
#include "util/NormalId.h"
#include "util/Vector3int32.h"

namespace RBX
{
	class Extents
	{
	private:
		Vector3 low;
		Vector3 high;

	public:
		Extents(const Vector3int32& low, const Vector3int32& high);
		Extents(const Vector3& low, const Vector3& high) : low(low), high(high) {}
		Extents() : low(Vector3::inf()), high(-Vector3::inf()) {}

	public:
		bool operator==(const Extents&) const;
		bool operator!=(const Extents&) const;
		Extents& operator=(const Extents&);
		const Vector3& min() const;
		const Vector3& max() const;
		Vector3 getCorner(int i) const;
		Vector3 size() const { return this->high - this->low; }
		Vector3 center() const { return (this->high + this->low) * 0.5f; }
		Vector3 bottomCenter() const;
		Vector3 topCenter() const;
		float longestSide() const;
		float volume() const;
		float areaXZ() const;
		Extents toWorldSpace(const CoordinateFrame&);
		Extents express(const CoordinateFrame&, const CoordinateFrame&);
		Vector3 faceCenter(NormalId faceId) const;
		void getFaceCorners(NormalId faceId, Vector3& v0, Vector3& v1, Vector3& v2, Vector3& v3) const;
		Plane getPlane(NormalId normalId) const;
		Vector3 clip(const Vector3&) const;
		Vector3 clamp(const Extents&) const;
		NormalId closestFace(const Vector3& point);
		void unionWith(const Extents& other);
		void shift(const Vector3&);
		void scale(float);
		void expand(float);
		Vector3& operator[](int);
		const Vector3& operator[](int) const;
		operator Vector3 *();
		operator const Vector3 *() const;
		bool contains(const Vector3& point) const;
		bool overlapsOrTouches(const Extents& other) const;
		bool fuzzyContains(const Vector3& point, float slop) const;
		bool containedByFrustum(const GCamera::Frustum& frustum) const;
		bool partiallyContainedByFrustum(const GCamera::Frustum&) const;
		bool separatedByMoreThan(const Extents& other, float distance) const;

	private:
		static float epsilon();
	public:
		static Extents fromCenterCorner(const Vector3&, const Vector3&);
		static Extents fromCenterRadius(const Vector3&, float);
		static Extents vv(const Vector3& v0, const Vector3& v1);
		static bool overlapsOrTouches(const Extents&, const Extents&);
		static const Extents& zero();
		static const Extents& unit();
		static const Extents& infiniteExtents();
		static const Extents& negativeInfiniteExtents();
	};
}
