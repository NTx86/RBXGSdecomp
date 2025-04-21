#include <G3DAll.h>
#include "v8kernel/Constants.h"
#include "util/Debug.h"
#include "util/Math.h"
using namespace RBX;

const float Constants::MAX_LEGO_JOINT_FORCES_MEASURED[7] = {0.0f, 1.098f, 2.1340001f, 2.427f, 3.191f, 4.5710001f, 4.6810002f};

const int Constants::uiStepsPerSec() {return 30;}
const int Constants::worldStepsPerUiStep() {return 8;} // guess based on uiStepsPerSec and worldStepsPerSec
const int Constants::kernelStepsPerWorldStep() {return 19;}
const int Constants::worldStepsPerSec() {return 240;}
const int Constants::kernelStepsPerSec() {return 4560;}
const int Constants::kernelStepsPerUiStep() {return 152;}
const float Constants::uiDt() {return (float)1/uiStepsPerSec();}
const float Constants::worldDt() {return (float)1/worldStepsPerSec();}
const float Constants::kernelDt() {return (float)1/kernelStepsPerSec();}

const float Constants::getElasticMultiplier(float elasticity)
{
	if ( elasticity < 0.05f )
		return 0.28f;
	else if ( elasticity < 0.26f )
		return 0.42f;
	else if ( elasticity < 0.51f )
		return 0.57f;
	else if ( elasticity < 0.76f )
		return 0.8f;

	return 1.0f;
}

const float RBX::Constants::getKmsMaxJointForce(float grid1, float grid2)
{
	RBXAssert(std::abs(grid1 * 10.0f - G3D::iRound(grid1 * 10.0f)) < 0.01f);
	RBXAssert(std::abs(grid2 * 10.0f - G3D::iRound(grid2 * 10.0f)) < 0.01f);

	int grid1int = std::max(1, G3D::iRound(grid1));
	int grid2int = std::max(1, G3D::iRound(grid2));

	int overlap = std::max(grid1int, grid2int);
	int width = std::min(grid1int, grid2int);

	float maxJointForce;

	if (overlap < 7){
		maxJointForce = MAX_LEGO_JOINT_FORCES_MEASURED[overlap];// line 120
	}
	else {
		maxJointForce = MAX_LEGO_JOINT_FORCES_MEASURED[6] * (overlap * 0.14285715f * 4.6810002f);// line 125
	}

	maxJointForce *= 0.5f;
	maxJointForce *= width;
	return maxJointForce * 7500.0f;
}

__forceinline Vector3 getClippedSortedSize(const Vector3& v)
{
	return v.max(Vector3(1.0f, 1.0f, 1.0f));
}

const float Constants::getJointKMultiplier(const G3D::Vector3& clippedSortedSize, bool ball)
{
	RBXAssert(clippedSortedSize.y >= clippedSortedSize.x);
	RBXAssert(clippedSortedSize.z >= clippedSortedSize.y);
	RBXAssert(getClippedSortedSize(clippedSortedSize) == clippedSortedSize);

	Vector3int16 size(clippedSortedSize);

	if (ball)
	{
		RBXAssert(size.x >= 1);

		switch(size.x)
		{
		case 1:
			return 0.23f;
		case 2:
			return 1.49f;
		case 3:
			return 4.43f;
		case 4:
			return 11.5f;
		default:
			return (size.x * size.x * size.x) * 0.175f;
		}
	}

	switch (size.x)
	{
	case 1:
		switch (size.y)
		{
		case 1:
			switch (size.z)
			{
			case 1:
				return 0.91f;
			case 2:
				return 1.61f;
			case 3:
				return 2.0f;
			case 4:
				return 2.13f;
			default:
				return size.z * 0.4f;
			}
		case 2:
			switch (size.z)
			{
			case 2:
				return 3.5f;
			case 3:
				return 4.16f;
			case 4:
				return 4.79f;
			default:
				return size.z < 15.0f ? size.z * 0.9f : size.z * 0.75f;
			}
		case 3:
			return size.z < 7.0f ? size.z * 1.66f : size.z * 1.18f;
		case 4:
			return size.z < 7.0f ? size.z * 2.26f : size.z * 1.53f;
		default:
			return (size.y * 0.3f + 0.66f) * size.z;
		}
	case 2:
		switch (size.y)
		{
		case 2:
			switch (size.z)
			{
			case 2:
				return 7.34f;
			case 3:
				return 9.90f;
			case 4:
				return 11.22f;
			default:
				return size.z < 15.0f ? size.z * 1.9f : size.z * 1.5f;
			}
		case 3:
			switch (size.z)
			{
			case 3:
				return 15.0f;
			case 4:
				return 19.0f;
			default:
				return size.z < 15.0f ? size.z * 2.0f : size.z * 1.5f;
			}
		default:
			return (size.y * 0.66f) * size.z;
		}
	default:
		return (size.x * size.y * size.z) * 0.25f;
	}
}

/*inline G3D::Vector3& unkInline(const G3D::Vector3& vector)
{
	G3D::Vector3 clipVector = vector;

	if (clipVector.z < 1.0f)
		clipVector.z = 1.0f;

	if (clipVector.y < 1.0f)
		clipVector.y = 1.0f;

	if (clipVector.x < 1.0f)
		clipVector.x = 1.0f;

	return clipVector;
}*/

//not gonna match until getJointKMultiplier is matched
const float Constants::getJointK(const G3D::Vector3& gridSize, bool ball)
{
	G3D::Vector3 sortedSize = Math::sortVector3(gridSize);
	G3D::Vector3 clippedSize = getClippedSortedSize(sortedSize);

	float JointKMultiplier = getJointKMultiplier(clippedSize, ball);

	if (sortedSize.x < 1.0)
		JointKMultiplier *= sortedSize.x;
	
	return JointKMultiplier * 960000.0f;
}