#include "v8world/Contact.h"

namespace RBX
{
	Contact::Contact(Primitive* prim0, Primitive* prim1)
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
}