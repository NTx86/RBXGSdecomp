#include "v8tree/Verb.h"

namespace RBX
{
	void VerbContainer::setVerbParent(VerbContainer* parent)
	{
		this->parent = parent;
	}

	Verb* VerbContainer::getVerb(std::string name)
	{
		return getVerb(Name::lookup(name));
	}
}
