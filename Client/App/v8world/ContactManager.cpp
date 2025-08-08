#include "v8world/ContactManager.h"
#include "v8world/spatialHash.h" // TODO: move these out maybe?
#include "v8world/World.h"

namespace RBX
{
	void ContactManager::getPrimitivesTouchingExtents(const Extents& extents, const Primitive* ignore, G3D::Array<Primitive*>& found)
	{
		this->spatialHash->getPrimitivesTouchingExtents(extents, ignore, found);
	}

	void ContactManager::onNewPair(Primitive* p0, Primitive* p1)
	{
		Contact* contact = this->createContact(p0, p1);
		this->world->insertContact(contact);
	}

	void ContactManager::onReleasePair(Primitive* p0, Primitive* p1)
	{
		this->world->destroyContact(Primitive::getContact(p0, p1));
	}

	void ContactManager::onPrimitiveAdded(Primitive* p)
	{
		this->spatialHash->onPrimitiveAdded(p);
	}

	void ContactManager::onPrimitiveRemoved(Primitive* p)
	{
		this->spatialHash->onPrimitiveRemoved(p);
	}

	void ContactManager::onPrimitiveExtentsChanged(Primitive* p)
	{
		this->spatialHash->onPrimitiveExtentsChanged(p);
	}

	void ContactManager::stepWorld()
	{
		this->spatialHash->onAllPrimitivesMoved();
	}
}