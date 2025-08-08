#include "v8world/Contact.h"
#include "util/PV.h"
#include "util/Math.h"

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

	float BlockBlockContact::contactPairHitRatio()
	{
		int sum = Contact::contactPairMatches + Contact::contactPairMisses;
		if (sum == 0)
			return -1.0f;
		else
			return (float)Contact::contactPairMatches / sum;
	}
}