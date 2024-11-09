#pragma once
#include "v8world/Geometry.h"
#include "v8kernel/Pair.h"

namespace RBX
{
	class Block : public Geometry
	{
	private:
		const G3D::Vector3* vertices;
		float cornerRadius;
	private:
		static const int BLOCK_FACE_TO_VERTEX[6][4];
		static const int BLOCK_FACE_VERTEX_TO_EDGE[6][4];
	private:
		const G3D::Vector3* getCornerPoint(const G3D::Vector3int16& clip) const;
		const G3D::Vector3* getEdgePoint(const G3D::Vector3int16& clip, NormalId& normalID) const;
		const G3D::Vector3* getPlanePoint(const G3D::Vector3int16& clip, NormalId& normalID) const;
		G3D::Matrix3 getMomentSolid(float) const;
		G3D::Matrix3 getMomentHollow(float) const;
		virtual void onSetSize();
	public:
		Block(const Block& other);
		Block();
		virtual ~Block() {}
		virtual bool hitTest(const G3D::Ray&, G3D::Vector3&, bool&);
		virtual GeometryType getGeometryType() const
		{
			return GEOMETRY_BLOCK;
		}
		virtual float getRadius() const;
		virtual G3D::Vector3 getCenterToCorner(const G3D::Matrix3&) const;
		virtual G3D::Matrix3 getMoment(float mass) const;
		virtual float getGridVolume() const;
		void projectToFace(G3D::Vector3& ray, G3D::Vector3int16& clip, int& onBorder);
		GeoPairType getBallInsideInfo(const G3D::Vector3& ray, const G3D::Vector3*& offset, NormalId& normalID);
		GeoPairType getBallBlockInfo(int onBorder, const G3D::Vector3int16 clip, const G3D::Vector3*& offset, NormalId& normalID);
		const float* getVertices() const { return (float*)this->vertices; }
		const G3D::Vector3& getExtent() const;
		const G3D::Vector3* getFaceVertex(NormalId, int) const;
		int getClosestEdge(const G3D::Matrix3& rotation, NormalId normalID, G3D::Vector3& crossAxis);
		int faceVertexToEdge(NormalId, int);
		int faceVertexToClockwiseEdge(NormalId, int);
		const G3D::Vector3* getEdgeVertex(int edgeId) const;
		NormalId getEdgeNormal(int);
		G3D::Vector2 getProjectedVertex(const G3D::Vector3& vertex, NormalId normalID);
		Block& operator=(const Block& other);

		// TODO: remove
		void matchDummy();
	};
}