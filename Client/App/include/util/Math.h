#pragma once
#include <G3DAll.h>

namespace RBX 
{
	// TODO: move this into it's correct file
	enum NormalId : int {
	NORM_X = 0x0000,
	NORM_Y = 0x0001,
	NORM_Z = 0x0002,
	NORM_X_NEG = 0x0003,
	NORM_Y_NEG = 0x0004,
	NORM_Z_NEG = 0x0005,
	NORM_UNDEFINED = 0x0006,
	};

	class Math { /* Size=0x1 */
		public:
			static const int maxOrientationId;
			static const int minOrientationId;

			static const float pi() {return 3.1415927f;}
			static const float piHalf() {return 1.5707964f;}
			static const float twoPi() {return 6.283185f;}
			static const float& inf()
			{
				static float inf = std::numeric_limits<float>::infinity();
				return inf;
			}
			static inline int iRound(float fvalue) {return G3D::iRound(fvalue);}
			static int iFloor(float);
			static float polarity(float);
			static float sign(float);
			static bool isDenormal(float);
			static bool isNanInfDenorm(float);
			static bool isNanInfDenormVector3(const G3D::Vector3&);
			static bool isNanInfDenormMatrix3(const G3D::Matrix3&);
			static bool fixDenorm(G3D::Vector3&);
			static bool fixDenorm(float&);
			static bool fuzzyEq(const G3D::CoordinateFrame&, const G3D::CoordinateFrame&, float, float);
			static bool fuzzyEq(const G3D::Matrix3&, const G3D::Matrix3&, float);
			static bool fuzzyEq(const G3D::Vector3&, const G3D::Vector3&, float);
			static bool fuzzyEq(float, float, float);
			static bool fuzzyAxisAligned(const G3D::Matrix3&, const G3D::Matrix3&, float);
			static bool isEven(int);
			static bool isOdd(int);
			static int nextEven(int);
			static int nextOdd(int);
			static float angleToE0(const G3D::Vector2&);
			static size_t hash(const G3D::Vector3&);
			static bool isIntegerVector3(const G3D::Vector3&);
			static G3D::Vector3 iRoundVector3(const G3D::Vector3&);
			static float angle(const G3D::Vector3&, const G3D::Vector3&);
			static float smallAngle(const G3D::Vector3&, const G3D::Vector3&);
			static float elevationAngle(const G3D::Vector3&);
			static G3D::Vector3 vector3Abs(const G3D::Vector3&);
			static float volume(const G3D::Vector3&);
			static float maxAxisLength(const G3D::Vector3&);
			static G3D::Vector3 sortVector3(const G3D::Vector3&);
			//static RBX::Velocity calcTrajectory(const G3D::Vector3&, const G3D::Vector3&, float);
			static G3D::Vector3 toGrid(const G3D::Vector3&, float);
			static G3D::Vector3 toGrid(const G3D::Vector3&, const G3D::Vector3&);
			static bool lessThan(const G3D::Vector3&, const G3D::Vector3&);
			static __forceinline float longestVector3Component(const G3D::Vector3& v)
			{
				return std::max(fabs(v.x), std::max(fabs(v.y), fabs(v.z)));
			}
			static float planarSize(const G3D::Vector3&);
			static float taxiCabMagnitude(const G3D::Vector3& v) {return fabs(v.x) + fabs(v.y) + fabs(v.z);}
			static const G3D::Plane& yPlane();
			static G3D::Vector3 closestPointOnRay(const G3D::Ray&, const G3D::Ray&);
			static bool cameraSeesPoint(const G3D::Vector3&, const G3D::GCamera&);
			static bool legalCameraCoord(const G3D::CoordinateFrame&);
			static G3D::Vector3 toSmallAngles(const G3D::Matrix3&);
			static G3D::Matrix3 snapToAxes(const G3D::Matrix3&);
			static bool isOrthonormal(const G3D::Matrix3&);
			static bool orthonormalizeIfNecessary(G3D::Matrix3&);
			static G3D::Vector3 toFocusSpace(const G3D::Vector3&, const G3D::CoordinateFrame&);
			static G3D::Vector3 fromFocusSpace(const G3D::Vector3&, const G3D::CoordinateFrame&);
			static G3D::Vector3 toDiagonal(const G3D::Matrix3& m);
			static G3D::Matrix3 fromDiagonal(const G3D::Vector3&);
			static G3D::Vector3 getColumn(const G3D::Matrix3&, int);
			static unsigned char rotationToByte(float);
			static float rotationFromByte(unsigned char);
			static bool isAxisAligned(const G3D::Matrix3&);
			static int getOrientId(const G3D::Matrix3&);
			static void idToMatrix3(int, G3D::Matrix3&);
			static const G3D::Matrix3& matrixRotateY();
			static const G3D::Matrix3& matrixRotateNegativeY();
			static const G3D::Matrix3& matrixTiltZ();
			static const G3D::Matrix3& matrixTiltNegativeZ();
			static const G3D::Matrix3 matrixTiltQuadrant(int);
			static void rotateMatrixAboutX90(G3D::Matrix3&, int);
			static void rotateMatrixAboutY90(G3D::Matrix3&, int);
			static void rotateMatrixAboutZ90(G3D::Matrix3&);
			static G3D::Matrix3 rotateAboutZ(const G3D::Matrix3&, float);
			static G3D::Matrix3 momentToObjectSpace(const G3D::Matrix3&, const G3D::Matrix3&);
			static G3D::Matrix3 momentToWorldSpace(const G3D::Matrix3& iBody, const G3D::Matrix3& bodyRotation);
			static G3D::Matrix3 getIWorldAtPoint(const G3D::Vector3&, const G3D::Vector3&, const G3D::Matrix3&, float);
			static void rotateAboutYLocal(G3D::CoordinateFrame&, float);
			static void rotateAboutYGlobal(G3D::CoordinateFrame&, float);
			static G3D::CoordinateFrame snapToGrid(const G3D::CoordinateFrame&, const G3D::Vector3&);
			static G3D::CoordinateFrame snapToGrid(const G3D::CoordinateFrame&, float);
			static float zAxisAngle(const G3D::CoordinateFrame&);
			static void pan(const G3D::Vector3&, G3D::CoordinateFrame&, float);
			static int radiansToQuadrant(float);
			static int radiansToOctant(float);
			static float radiansToDegrees(float);
			static float degreesToRadians(float);
			static float getHeading(const G3D::Vector3&);
			static float getElevation(const G3D::Vector3&);
			static void getHeadingElevation(const G3D::CoordinateFrame&, float&, float&);
			static void setHeadingElevation(G3D::CoordinateFrame&, float, float);
			static G3D::CoordinateFrame getFocusSpace(const G3D::CoordinateFrame&);
			static int toYAxisQuadrant(const G3D::CoordinateFrame&);
			static G3D::Matrix3 alignAxesClosest(const G3D::Matrix3&, const G3D::Matrix3&);
			static RBX::NormalId getClosestObjectNormalId(const G3D::Vector3&, const G3D::Matrix3&);
			static G3D::Vector3 getWorldNormal(RBX::NormalId, const G3D::CoordinateFrame&);
			static G3D::Vector3 getWorldNormal(RBX::NormalId, const G3D::Matrix3&);
			static float radWrap(float);
			static const G3D::Matrix3& getAxisRotationMatrix(int);
			static G3D::Vector3 vectorToObjectSpace(const G3D::Vector3&, const G3D::Matrix3&);
			static bool clipRay(G3D::Vector3&, G3D::Vector3&, G3D::Vector3*, G3D::Vector3&);
			static bool intersectLinePlane(const G3D::Line&, const G3D::Plane&, G3D::Vector3&);
			static bool intersectRayPlane(const G3D::Ray&, const G3D::Plane&, G3D::Vector3&);
			static float computeLaunchAngle(float, float, float, float);
	};
}