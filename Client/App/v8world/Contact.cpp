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

	void Contact::deleteConnector(ContactConnector*& c)
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
}