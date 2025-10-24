#pragma once

#include "util/ContentProvider.h"

namespace RBX
{
	class MeshId : public ContentId {
	public:
		MeshId(const MeshId&);
		MeshId();
		MeshId(const std::string&);
		MeshId(const char*);
		MeshId(const ContentId&);
		~MeshId();
		MeshId& operator=(const MeshId&);
	};
}