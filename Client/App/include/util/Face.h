#pragma once
#include <G3DAll.h>
#include "util/Extents.h"

namespace RBX
{
	class Face
	{
	private:
		G3D::Vector3 c0;
		G3D::Vector3 c1;
		G3D::Vector3 c2;
		G3D::Vector3 c3;

	public:
		//Face(const Face&);
		Face();
		Face(const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector3&, const G3D::Vector3&);
	private:
		G3D::Vector3 getAxis(int) const;
		void minMax(const G3D::Vector3&, const G3D::Vector3&, float&, float&) const;
		Face operator*(const G3D::Vector3&) const;
		Face operator*(float) const;
	public:
		void snapToGrid(float);
		const G3D::Vector3& operator[](int) const;
		G3D::Vector3& operator[](int);
		G3D::Vector3 getU() const;
		G3D::Vector3 getV() const;
		G3D::Vector3 getNormal() const;
		G3D::Vector2 size() const;
		G3D::Vector3 center() const;
		Face toWorldSpace(const G3D::CoordinateFrame&) const;
		Face toObjectSpace(const G3D::CoordinateFrame&) const;
		Face projectOverlapOnMe(const Face&) const;
		bool fuzzyContainsInExtrusion(const G3D::Vector3&, float) const;
		Face& operator=(const Face&);

	public:
		static Face fromExtentsSide(const Extents&, NormalId);
		static bool cornersAligned(const Face&, const Face&, float);
		static bool hasOverlap(const Face&, const Face&, float);
		static bool overlapWithinPlanes(const Face&, const Face&, float);
	};
}
