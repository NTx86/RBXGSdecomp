#include "v8world/Contact.h"
#include "util/PV.h"
#include "util/Math.h"
#include "util/StlExtra.h"

namespace RBX
{
	__declspec(noinline) Contact::Contact(Primitive* prim0, Primitive* prim1)
		: Edge(prim0, prim1),
		jointK(0),
		elasticJointK(0),
		lastContactStep(-1),
		steppingIndex(-1),
		kFriction(0)
	{
	}

	void Contact::putInKernel(Kernel* _kernel)
	{
		IPipelined::putInKernel(_kernel);
		onPrimitiveContactParametersChanged();
	}

	void Contact::removeFromKernel()
	{
		RBXASSERT(IPipelined::getKernel());

		deleteAllConnectors();
		IPipelined::removeFromKernel();
	}

	ContactConnector* Contact::createConnector()
	{
		ContactConnector* contact = new ContactConnector(this->jointK, this->elasticJointK, this->kFriction);
		this->getKernel()->insertConnector(contact);

		return contact;
	}

	__declspec(noinline) void Contact::deleteConnector(ContactConnector*& c)
	{
		if (c)
		{
			this->getKernel()->removeConnector(c);
			delete c;
			c = NULL;
		}
	}

	//needed because of header inlining bullshit
	void Contact::deleteConnectorInline(ContactConnector*& c)
	{
		if (c)
		{
			this->getKernel()->removeConnector(c);
			delete c;
			c = NULL;
		}
	}

	bool Contact::computeIsAdjacent(float spaceAllowed)
	{
		if (this->computeIsColliding(spaceAllowed))
			return false;
		else
			return this->computeIsColliding(-spaceAllowed);
	}

	bool Contact::step(int uiStepId)
	{
		RBXASSERT(uiStepId >= 0);
		
		bool result = this->stepContact();
		
		if (result)
		{
			if (this->lastContactStep == -1 ) 
				Primitive::onNewTouch(Edge::getPrimitive(0), Edge::getPrimitive(1));

			this->lastContactStep = uiStepId;
		}
		else if (this->lastContactStep < uiStepId)
		{
			this->lastContactStep = -1;
		}

		return result;
	}

	void Contact::onPrimitiveContactParametersChanged()
	{
		Primitive* prim0 = Edge::getPrimitive(0);
		Primitive* prim1 = Edge::getPrimitive(1);
		
		this->kFriction = std::min(prim0->getFriction(), prim1->getFriction());
		float elasticity = std::min(prim0->getElasticity(), prim1->getElasticity());

		this->jointK = std::min(prim0->getJointK(), prim1->getJointK());
		this->elasticJointK = Constants::getElasticMultiplier(elasticity) * this->jointK;
	}

	BallBallContact::BallBallContact(Primitive* p0, Primitive* p1)
		:Contact(p0, p1),
		ballBallConnector(NULL) {}

	BallBallContact::~BallBallContact()
	{
		RBXASSERT(!this->ballBallConnector);
	}

	Ball* BallBallContact::ball(int i)
	{
		return rbx_static_cast<Ball*>(this->getPrimitive(i)->getGeometry());
	}

	void BallBallContact::deleteAllConnectors()
	{
		this->deleteConnector(this->ballBallConnector);
	}

	bool BallBallContact::computeIsColliding(float overlapIgnored)
	{
		float b0Radius = this->ball(0)->getRadius();
		float b1Radius = this->ball(1)->getRadius();

		Vector3 delta = this->getPrimitive(1)->getBody()->getPV().position.translation - this->getPrimitive(0)->getBody()->getPV().position.translation;
		float b0b1RadiusSum = b0Radius + b1Radius;

		if (b0b1RadiusSum > Math::longestVector3Component(delta))
			return delta.magnitude() < (b0b1RadiusSum - overlapIgnored);
		else
			return false;
	}

	bool BallBallContact::stepContact()
	{
		if (BallBallContact::computeIsColliding(0.0f))
		{
			if (this->inStage(IStage::KERNEL_STAGE))
			{
				if (!this->ballBallConnector)
					this->ballBallConnector = this->createConnector();

				this->ballBallConnector->setBallBall(
					this->getPrimitive(0)->getBody(), 
					this->getPrimitive(1)->getBody(), 
					this->ball(0)->getRadius(), 
					this->ball(1)->getRadius()
					);
			}
			return true;
		}
		else
		{
			this->deleteAllConnectors();
		}

		return false;
	}

	BallBlockContact::BallBlockContact(Primitive* p0, Primitive* p1)
		:Contact(p0, p1),
		ballBlockConnector(NULL) {}

	BallBlockContact::~BallBlockContact()
	{
		RBXASSERT(!this->ballBlockConnector);
	}

	Primitive* BallBlockContact::ballPrim()
	{	
		return this->getPrimitive(0);
	}

	Primitive* BallBlockContact::blockPrim()
	{
		return this->getPrimitive(1);
	}

	Ball* BallBlockContact::ball()
	{
		return rbx_static_cast<Ball*>(this->ballPrim()->getGeometry());
	}

	Block* BallBlockContact::block()
	{
		return rbx_static_cast<Block*>(this->blockPrim()->getGeometry());
	}

	bool BallBlockContact::computeIsColliding(int& onBoarder, G3D::Vector3int16& clip, G3D::Vector3& projectionInBlock, float overlapIgnored)
	{
		if (Primitive::aaBoxCollide(*this->ballPrim(), *this->blockPrim()))
		{
			Body* b0 = this->ballPrim()->getBody();
			Body* b1 = this->blockPrim()->getBody();

			//const CoordinateFrame& prim0Coord = b0->getPV().position;
			//const CoordinateFrame& prim1Coord = b1->getPV().position;
			const PV& prim0Coord = b0->getPV();
			const PV& prim1Coord = b1->getPV();

			Vector3& blockToBall = prim0Coord.position.translation - prim1Coord.position.translation;
			projectionInBlock = prim1Coord.position.rotation.transpose() * blockToBall; //could be some sort to objectSpace inline but operator* inlines when it shouldn't
			
			this->block()->projectToFace(projectionInBlock, clip, onBoarder);
			Vector3& unkVec = prim0Coord.position.pointToObjectSpace(projectionInBlock);
			Vector3& unkVec2 = unkVec - prim0Coord.position.translation;

			return unkVec2.magnitude() < (this->ball()->getRadius() - overlapIgnored);
		}
		return false;
	}

	bool BallBlockContact::computeIsColliding(float overlapIgnored)
	{
		Vector3int16 clip;
		Vector3 projectionInBlock;
		return this->computeIsColliding(*(int*)&overlapIgnored, clip, projectionInBlock, overlapIgnored);
	}

	bool BallBlockContact::stepContact()
	{
		Vector3int16 clip;
		Vector3 projectionInBlock;
		int onBoarder;

		if (BallBlockContact::computeIsColliding(onBoarder, clip, projectionInBlock, 0.0f))
		{
			if (this->inStage(IStage::KERNEL_STAGE))
			{
				if (!this->ballBlockConnector)
					this->ballBlockConnector = this->createConnector();

				const Vector3* offset;
				NormalId normId;
				GeoPairType ballInsideType;

				if (onBoarder)
					ballInsideType = this->block()->getBallBlockInfo(onBoarder, clip, offset, normId);
				else
					ballInsideType = this->block()->getBallInsideInfo(projectionInBlock, offset, normId);

				this->ballBlockConnector->setBallBlock(
					this->ballPrim()->getBody(),
					this->blockPrim()->getBody(),
					this->ball()->getRadius(),
					offset,
					normId,
					ballInsideType
					);
			}
			return true;
		}
		else
		{
			this->deleteAllConnectors();
		}

		return false;
	}

	void BallBlockContact::deleteAllConnectors()
	{
		this->deleteConnector(this->ballBlockConnector);
	}

	BlockBlockContact::BlockBlockContact(Primitive* p0, Primitive* p1)
		:Contact(p0, p1),
		separatingAxisId(0),
		separatingBodyId(0)
	{
		this->feature[0] = -1;
		this->feature[1] = -1;
	}
	
	BlockBlockContact::~BlockBlockContact() {}

	Block* BlockBlockContact::block(int i)
	{
		return rbx_static_cast<Block*>(this->getPrimitive(i)->getGeometry());
	}

	float BlockBlockContact::contactPairHitRatio()
	{
		int sum = Contact::contactPairMatches + Contact::contactPairMisses;

		if (sum == 0)
			return -1.0f;
		else
			return (float)Contact::contactPairMatches / sum;
	}

	void BlockBlockContact::deleteAllConnectors()
	{
		for (size_t i = 0; i < this->connectors.size(); i++)
		{
			this->deleteConnectorInline(this->connectors[i]);
		}

		this->connectors.resize(0);
		this->matched.resize(0);
	}

	void BlockBlockContact::deleteUnmatchedConnectors()
	{
		RBXASSERT(this->matched.size() == this->connectors.size());

		for (int i = (int)this->connectors.size()-1; i >= 0; i--)
		{
			bool match = this->matched[i];
			if (match == false)
			{
				ContactConnector* connector = this->connectors[i];
				fastRemoveIndex<ContactConnector*>(this->connectors, i);
				fastRemoveIndex<bool>(this->matched, i);
				this->deleteConnectorInline(connector);
			}
		}
	}

	ContactConnector* BlockBlockContact::matchContactConnector(Body* b0, Body* b1, GeoPairType _pairType, int param0, int param1)
	{
		RBXASSERT(this->matched.size() == this->connectors.size());

		for (size_t i = 0; i < this->matched.size(); i++)
		{
			if (this->matched[i] == false)
			{
				if (this->connectors[i]->match(b0, b1, _pairType, param0, param1))
				{
					this->matched[i] = true;
					Contact::contactPairMatches++;
					return this->connectors[i];
				}
			}
		}

		Contact::contactPairMisses++;
		ContactConnector* connector = this->createConnector();
		this->connectors.push_back(connector);
		this->matched.push_back(true);
		return connector;
	}

	void BlockBlockContact::loadGeoPairEdgeEdge(int b0, int b1, int edge0, int edge1)
	{
		NormalId edgeNormal0 = this->block(b0)->getEdgeNormal(edge0);
		NormalId edgeNormal1 = this->block(b1)->getEdgeNormal(edge1);
		
		ContactConnector* matched = this->matchContactConnector(
											this->getPrimitive(b0)->getBody(), 
											this->getPrimitive(b1)->getBody(), 
											EDGE_EDGE_PAIR, 
											edgeNormal0, 
											edgeNormal1
											);

		matched->setEdgeEdge(
			this->getPrimitive(b0)->getBody(), 
			this->getPrimitive(b1)->getBody(), 
			this->block(b0)->getEdgeVertex(edge0), 
			this->block(b1)->getEdgeVertex(edge1), 
			edgeNormal0, 
			edgeNormal1
			);
	}

	void BlockBlockContact::loadGeoPairPointPlane(int pointBody, int planeBody, int pointID, NormalId pointFaceID, NormalId planeFaceID)
	{
		ContactConnector* matched = this->matchContactConnector(
											this->getPrimitive(pointBody)->getBody(), 
											this->getPrimitive(planeBody)->getBody(), 
											POINT_PLANE_PAIR, 
											pointID, 
											planeFaceID
											);

		matched->setPointPlane(
			this->getPrimitive(pointBody)->getBody(),
			this->getPrimitive(planeBody)->getBody(),
			this->block(pointBody)->getFaceVertex(pointFaceID, pointID),
			this->block(planeBody)->getFaceVertex(planeFaceID, 0),
			pointID,
			planeFaceID
			);
	}

	void BlockBlockContact::loadGeoPairEdgeEdgePlane(int edgeBody, int planeBody, int edge0, int edge1)
	{
		NormalId edgeNormal0 = this->block(edgeBody)->getEdgeNormal(edge0);
		NormalId edgeNormal1 = this->block(planeBody)->getEdgeNormal(edge1);

		ContactConnector* matched = this->matchContactConnector(
											this->getPrimitive(edgeBody)->getBody(), 
											this->getPrimitive(planeBody)->getBody(), 
											EDGE_EDGE_PLANE_PAIR, 
											edgeNormal0, 
											edgeNormal1
											);

		const Vector3& gridSize = this->getPrimitive(edgeBody)->getGeometry()->getGridSize();

		matched->setEdgeEdgePlane(
			this->getPrimitive(edgeBody)->getBody(),
			this->getPrimitive(planeBody)->getBody(),
			this->block(edgeBody)->getEdgeVertex(edge0),
			this->block(planeBody)->getEdgeVertex(edge1),
			edgeNormal0,
			edgeNormal1,
			this->planeID,
			gridSize[edgeNormal0 % 3]
			);
	}

	//this is hell (85% match)
	//PLEASE NOTE THAT MOST VARIABLE NAMES ARE MOST LIKELY NOT ACCURATE OF THEIR FUNCTIONALITY
	bool BlockBlockContact::getBestPlaneEdge(bool& planeContact, float overlapIgnored)
	{
		float bestPlaneLength = Math::inf();
		float bestEdgeLength = Math::inf();
		float unkMathInf = Math::inf();
		int featureSave[2] = {this->feature[0], this->feature[1]};
		bool checkLastFeature = (featureSave[0] >= 0 && featureSave[0] < 6) || featureSave[1] <= 5;

		int sepIdCounter = this->separatingBodyId + 1;

		for (int i = this->separatingBodyId; i < this->separatingBodyId + 2; i++)
		{
			int baseId = this->separatingBodyId % 2;
			int testId = sepIdCounter % 2;
			const CoordinateFrame& primPV0 = this->getPrimitive(baseId)->getBody()->getPV().position;
			const CoordinateFrame& primPV1 = this->getPrimitive(testId)->getBody()->getPV().position;
			Vector3* vertices0 = (Vector3*)this->block(0)->getVertices();
			Vector3* vertices1 = (Vector3*)this->block(1)->getVertices();

			Vector3 delta = primPV1.translation - primPV0.translation;

			Vector3 rotTransMul = primPV0.rotation * delta;

			for (int j = this->separatingAxisId; j < this->separatingAxisId + 3; j++)
			{
				int jRemain = j % 3;

				float what = Math::taxiCabMagnitude(primPV1.rotation * primPV0.rotation.getColumn(jRemain) * *vertices0) + *vertices1[jRemain]  - fabs(rotTransMul[jRemain]);

				if (overlapIgnored > what)
				{
					if (checkLastFeature && featureSave[baseId] % 3 == jRemain )
						unkMathInf = what;

					if (bestPlaneLength < what)
					{
						bestPlaneLength = what;
						this->feature[baseId] = rotTransMul[jRemain] > 0.0f ? jRemain : jRemain + 3;
						this->feature[testId] = -1;
						this->separatingBodyId = baseId;
						this->separatingAxisId = jRemain;
						planeContact = true;
					}
				}
				else
				{
					this->separatingBodyId = baseId;
					this->separatingAxisId = jRemain;
					return false;
				}
			}
			sepIdCounter++;
		}


		if (checkLastFeature && (this->feature[0] != featureSave[0] || this->feature[1] != featureSave[1]) && !(bestPlaneLength * 1.01f < unkMathInf))
		{
			bestPlaneLength = unkMathInf;
			this->feature[0] = featureSave[0];
			this->feature[1] = featureSave[1];
		}
		const CoordinateFrame& primPV0 = this->getPrimitive(0)->getBody()->getPV().position;
		const CoordinateFrame& primPV1 = this->getPrimitive(1)->getBody()->getPV().position;
		Vector3* vertices0 = (Vector3*)this->block(0)->getVertices();
		Vector3* vertices1 = (Vector3*)this->block(1)->getVertices();

		Vector3 delta = primPV1.translation - primPV0.translation;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vector3 PVcrossProd = primPV0.rotation.getColumn(i).cross(primPV1.rotation.getColumn(j));
				if (PVcrossProd.unitize() <= 0.001f)
					return planeContact;

				float p0p1inCrossAxis = delta.dot(PVcrossProd);

				Vector3 PVcrossProdMulPV0rot = primPV0.rotation * PVcrossProd;
				Vector3 PVcrossProdMulPV1rot = primPV1.rotation * PVcrossProd;

				float what = Math::taxiCabMagnitude(PVcrossProdMulPV0rot * *vertices0) + Math::taxiCabMagnitude(PVcrossProdMulPV1rot * *vertices1) - fabs(p0p1inCrossAxis);
				if (overlapIgnored > what)
				{
					if (bestEdgeLength < what)
					{
						if ( what * 10.0 < bestPlaneLength )
						{
							if ( p0p1inCrossAxis > 0.0 )
							{
								this->feature[0] = this->block(0)->getClosestEdge(primPV0.rotation, (NormalId)i, PVcrossProd) + 6;
								this->feature[1] = this->block(1)->getClosestEdge(primPV1.rotation, (NormalId)j, -PVcrossProd) + 6;
							}
							else
							{
								this->feature[0] = this->block(0)->getClosestEdge(primPV0.rotation, (NormalId)i, -PVcrossProd) + 6;
								this->feature[1] = this->block(1)->getClosestEdge(primPV1.rotation, (NormalId)j, PVcrossProd) + 6;
							}
							planeContact = false;
						}
					}
				}
				else
				{
					 return false;
				}
			}
		}
		return true;
	}
}