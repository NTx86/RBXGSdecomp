#include <G3DAll.h>
#include "constants.h"
#include "debug.h"
#include "math.h"
using namespace RBX;

const float Constants::MAX_LEGO_JOINT_FORCES_MEASURED[7] = {0.0f, 1.098f, 2.1340001f, 2.427f, 3.191f, 4.5710001f, 4.6810002f};

const int Constants::kernelStepsPerWorldStep(){return 19;}
const int Constants::worldStepsPerSec() {return 240;}
const int Constants::kernelStepsPerSec() {return 4560;}
const int Constants::kernelStepsPerUiStep() {return 152;}
const float Constants::uiDt() {return 0.033333335f;}
const float Constants::worldDt() {return 0.0041666669f;}
const float Constants::kernelDt() {return 0.00021929825f;}

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

//random crap so getJointK would match better
//i cant even attempt this right now because i can't export this function as a obj
const float Constants::getJointKMultiplier(const G3D::Vector3& clippedSortedSize, bool ball)
{
	float result = 12;
	float good2 = 96;
	float ughhhh = 0.434343f;
	G3D::Vector3 moredummy = Vector3(1.6f, 1.2f, 1.7f);

	for (int i = 0; i < 7; i++)
	{
		result *= MAX_LEGO_JOINT_FORCES_MEASURED[i];
		good2 += good2 * result / i;
		ughhhh = ughhhh * clippedSortedSize.y / clippedSortedSize.z + ball;
		result /= ughhhh;
		result *= good2;
		moredummy.y += good2 + moredummy.z;
	}
	return result + moredummy.y / good2 + clippedSortedSize.x / ughhhh + clippedSortedSize.y + clippedSortedSize.z * MAX_LEGO_JOINT_FORCES_MEASURED[6] / 500.0f;
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
	G3D::Vector3 clippedSize = sortedSize.max(Vector3(1.0f, 1.0f, 1.0f)); //spent way too much time on this

	float JointKMultiplier = getJointKMultiplier(clippedSize, ball);

	if (sortedSize.x < 1.0)
		JointKMultiplier *= sortedSize.x;
	
	return JointKMultiplier * 960000.0f;
}